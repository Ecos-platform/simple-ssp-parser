
#include "util/temp_dir.hpp"
#include "util/unzipper.hpp"

#include "ssp/ssp.hpp"
#include "ssp/util/fs_portability.hpp"

#include <pugixml.hpp>

using namespace ssp;


Connection parse_connection(const pugi::xml_node& node)
{
    const auto startElement = node.attribute("startElement").as_string();
    const auto startConnector = node.attribute("startConnector").as_string();
    const auto endElement = node.attribute("endElement").as_string();
    const auto endConnector = node.attribute("endConnector").as_string();
    Connection c = {startElement, startConnector, endElement, endConnector};
    const auto transformationNode = node.child("ssc:LinearTransformation");
    if (transformationNode) {
        double factor = transformationNode.attribute("factor").as_double();
        double offset = transformationNode.attribute("offset").as_double();
        c.linearTransformation = {factor, offset};
    }
    return c;
}

std::vector<Connection> parse_connections(const pugi::xml_node& node)
{
    std::vector<Connection> connections;
    for (const auto c : node) {
        connections.emplace_back(parse_connection(c));
    }
    return connections;
}

Connector parse_connector(const pugi::xml_node& node)
{
    const auto connectorName = node.attribute("name").as_string();
    const auto connectorKind = node.attribute("kind").as_string();
    Connector connector = {connectorName, connectorKind};
    if (node.child("ssc:Real")) {
        connector.type.real = 0;
    } else if (node.child("ssc:Integer")) {
        connector.type.integer = 0;
    } else if (node.child("ssc:Boolean")) {
        connector.type.boolean = false;
    } else if (node.child("ssc:String")) {
        connector.type.string = "";
    }
    return connector;
}

std::unordered_map<std::string, Connector> parse_connectors(const pugi::xml_node& node)
{
    std::unordered_map<std::string, Connector> connectors;
    for (const auto c : node) {
        const auto connector = parse_connector(c);
        connectors.emplace(connector.name, connector);
    }
    return connectors;
}

Parameter parse_parameter(const pugi::xml_node& node)
{
    const auto name = node.attribute("name").as_string();
    Parameter parameter{name};
    pugi::xml_node typeNode;
    if (node.child("ssv:Real")) {
        typeNode = node.child("ssv:Real");
        const auto value = typeNode.attribute("value").as_double();
        parameter.type.real = value;
    } else if (node.child("ssv:Integer")) {
        typeNode = node.child("ssv:Integer");
        const auto value = typeNode.child("ssv:Integer").attribute("value").as_int();
        parameter.type.integer = value;
    } else if (node.child("ssv:Boolean")) {
        typeNode = node.child("ssv:Boolean");
        const auto value = typeNode.child("ssv:Boolean").attribute("value").as_bool();
        parameter.type.boolean = value;
    } else if (node.child("ssv:String")) {
        typeNode = node.child("ssv:ssv:String");
        const auto value = typeNode.child("ssv:String").attribute("value").as_string();
        parameter.type.string = value;
    } else {
        throw std::runtime_error("Unknown XML node in ssv:Parameter encountered!");
    }
    const auto unit = typeNode.attribute("unit");
    if (!unit.empty()) {
        parameter.type.unit = unit.as_string();
    }
    return parameter;
}

std::unordered_map<std::string, ParameterSet>
parse_parameter_bindings(const fs::path& dir, const pugi::xml_node& node)
{
    std::unordered_map<std::string, ParameterSet> parameterSets;
    for (const auto parameterBindingNode : node) {
        pugi::xml_node parameterSetNode;
        std::unique_ptr<pugi::xml_document> doc;
        const auto parameterValues = parameterBindingNode.child("ssd:ParameterValues");
        if (parameterValues) {
            parameterSetNode = parameterValues.child("ssv:ParameterSet");
        } else {
            const auto source = parameterBindingNode.attribute("source").as_string();
            doc = std::make_unique<pugi::xml_document>();
            pugi::xml_parse_result result = doc->load_file(fs::path(dir / source).c_str());
            if (!result) {
                throw std::runtime_error(
                    "Unable to parse '" + fs::absolute(fs::path(dir / source)).string() + "': " + result.description());
            }
            parameterSetNode = doc->child("ssv:ParameterSet");
        }
        const auto name = parameterSetNode.attribute("name").as_string();
        ParameterSet set{name};
        const auto parametersNode = parameterSetNode.child("ssv:Parameters");
        for (const auto parameterNode : parametersNode) {
            Parameter p = parse_parameter(parameterNode);
            set.parameters.emplace_back(p);
        }
        parameterSets[name] = set;
    }
    return parameterSets;
}

Component parse_component(const fs::path& dir, const pugi::xml_node& node)
{
    const auto componentName = node.attribute("name").as_string();
    const auto componentSource = node.attribute("source").as_string();
    const auto connectors = parse_connectors(node.child("ssd:Connectors"));
    const auto parameterSets = parse_parameter_bindings(dir, node.child("ssd:ParameterBindings"));
    return {componentName, componentSource, connectors, parameterSets};
}

std::unordered_map<std::string, Component> parse_components(const fs::path& dir, const pugi::xml_node& node)
{
    std::unordered_map<std::string, Component> components;
    for (const auto childNode : node) {
        if (std::string(childNode.name()) == "ssd:Component") {
            Component c = parse_component(dir, childNode);
            components[c.name] = c;
        }
    }
    return components;
}

Elements parse_elements(const fs::path& dir, const pugi::xml_node& node)
{
    Elements elements;
    elements.components = parse_components(dir, node);

    // collect parameterSets by name
    for (const auto& [componentName, component] : elements.components) {
        for (const auto& [parameterSetName, parameterSet] : component.parameterSets) {
            auto& list = elements.parameterSets[parameterSetName][component];
            list.insert(list.end(), parameterSet.parameters.begin(), parameterSet.parameters.end());
        }
    }

    return elements;
}

std::vector<Annotation> parse_annotations(const pugi::xml_node& node)
{
    std::vector<Annotation> annotations;
    for (const auto& annotationNode : node) {
        const auto type = annotationNode.attribute("type").as_string();
        annotations.emplace_back(Annotation{type, annotationNode});
    }
    return annotations;
}

DefaultExperiment parse_default_experiment(const pugi::xml_node& node)
{
    const auto start = node.attribute("start");
    const auto stop = node.attribute("stop");
    DefaultExperiment ex;
    if (start) {
        ex.start = start.as_double();
    }
    if (stop) {
        ex.stop = stop.as_double();
    }
    const auto annotationsNode = node.child("ssd:Annotations");
    if (annotationsNode) {
        ex.annotations = parse_annotations(annotationsNode);
    }
    return ex;
}

System parse_system(const fs::path& dir, const pugi::xml_node& node)
{
    System sys;
    sys.name = node.attribute("name").as_string();
    sys.description = node.attribute("description").as_string();

    const auto elementsNode = node.child("ssd:Elements");
    sys.elements = parse_elements(dir, elementsNode);

    const auto connectionsNode = node.child("ssd:Connections");
    sys.connections = parse_connections(connectionsNode);

    return sys;
}

struct SystemStructureDescription::Impl
{

    std::string name;
    std::string version;

    System system;
    std::optional<DefaultExperiment> defaultExperiment;

    explicit Impl(const fs::path& path)
    {

        if (!fs::exists(path)) {
            throw std::runtime_error("No such file: " + absolute(path).string());
        }

        if (fs::is_directory(path)) {
            dir = path;
        } else {
            tmp = std::make_unique<temp_dir>("ssp");
            dir = tmp->path();
            if (!unzip(path, dir)) {
                throw std::runtime_error("Failed to unzip contents..");
            }
        }

        pugi::xml_parse_result result = doc_.load_file(fs::path(dir / "SystemStructure.ssd").c_str());
        if (!result) {
            throw std::runtime_error(
                "Unable to parse '" + fs::absolute(fs::path(dir / "SystemStructure.ssd")).string() + "': " +
                result.description());
        }

        const auto root = doc_.child("ssd:SystemStructureDescription");

        name = root.attribute("name").as_string();
        version = root.attribute("version").as_string();

        if (version != "1.0") {
            throw std::runtime_error("Unsupported SSP version: '" + version + "'!");
        }

        const auto systemNode = root.child("ssd:System");
        system = parse_system(dir, systemNode);

        const auto defaultNode = root.child("ssd:DefaultExperiment");
        if (defaultNode) {
            defaultExperiment = parse_default_experiment(defaultNode);
        }
    }

    [[nodiscard]] fs::path file(const fs::path& source) const
    {
        return dir / source;
    }

    ~Impl() = default;

private:
    fs::path dir;
    pugi::xml_document doc_;
    std::unique_ptr<temp_dir> tmp = nullptr;
};

SystemStructureDescription::SystemStructureDescription(const fs::path& path)
    : pimpl_(new Impl(path))
    , name(pimpl_->name)
    , version(pimpl_->version)
    , system(pimpl_->system)
    , defaultExperiment(pimpl_->defaultExperiment)
{ }

fs::path ssp::SystemStructureDescription::file(const fs::path& source) const
{
    return pimpl_->file(source);
}

SystemStructureDescription::~SystemStructureDescription() = default;
