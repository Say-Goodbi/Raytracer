#include "Parser.hpp"

RayTracer::Parser::Parser(const std::string &filename) : _root(nullptr)
{
    try {
            std::cout << "Loading configuration file: " << filename << std::endl;
        _cfg.readFile(filename.c_str());
        std::cout << "Configuration file loaded successfully: " << filename << std::endl;
        _root = &_cfg.getRoot();
        if (!_root)
            throw RayTracer::Exception("Failed to read configuration file: " + filename);
        _node = RayTracer::NodePtr(new RayTracer::Node);
        _node->value = RayTracer::Object{};
        RayTracer::Object &rootObj = std::get<RayTracer::Object>(_node->value);
        parseSettings(*_root, rootObj, "");
    }
    catch (const libconfig::FileIOException &e)
    {
        throw RayTracer::Exception("Failed to read configuration file: " + filename);
    }
    catch (const libconfig::ParseException &e)
    {
        throw RayTracer::Exception("Failed to parse configuration file: " + filename + " at line " + std::to_string(e.getLine()));
    }
}

RayTracer::Parser::~Parser()
{
    _cfg.clear();
}

/**
 * @brief Extracts a scalar value from a libconfig setting and stores it in a ScalarValue variant.
 * @param s The libconfig setting to extract the value from
 * @param result A reference to a ScalarValue variant where the extracted value will be stored
 * @return void
 */
void RayTracer::Parser::getValue(const libconfig::Setting &s, RayTracer::ScalarValue &result)
{
    switch (s.getType())
    {
    case libconfig::Setting::Type::TypeBoolean:
        result = s.operator bool();
        break;
    case libconfig::Setting::Type::TypeInt:
        result = s.operator int();
        break;
    case libconfig::Setting::Type::TypeInt64:
        result = s.operator long long();
        break;
    case libconfig::Setting::Type::TypeFloat:
        result = s.operator float();
        break;
    case libconfig::Setting::Type::TypeString:
        result = s.operator std::string();
        break;
    default:
        std::cerr << "Unsupported type for setting: " << s.getName() << " (type: " << s.getType() << ")\n";
    }
}

/**
 * @brief Inserts a value into the object structure, creating nested objects as needed.
 * @param obj The object to insert the value into
 * @param prefix The prefix for the value's name
 * @param s The libconfig setting to extract the value from
 * @return void
 */
void RayTracer::Parser::insert_value(RayTracer::Object &obj, const std::string &prefix, const libconfig::Setting &s)
{
    std::string name = prefix + s.getName();
    RayTracer::ScalarValue value{};
    getValue(s, value);
    size_t lastDot = name.rfind('.');

    if (lastDot != std::string::npos)
    {
        std::string parentName = name.substr(0, lastDot);
        std::string childName = name.substr(lastDot + 1);
        RayTracer::Object *parentObj = &obj;
        size_t dotPos = parentName.find('.');

        while (dotPos != std::string::npos)
        {
            std::string currentPart = parentName.substr(0, dotPos);
            if (parentObj->find(currentPart) == parentObj->end())
            {
                (*parentObj)[currentPart] = RayTracer::NodePtr(new RayTracer::Node{RayTracer::Object{}});
            }
            parentObj = &std::get<RayTracer::Object>((*parentObj)[currentPart]->value);
            parentName = parentName.substr(dotPos + 1);
            dotPos = parentName.find('.');
        }
        if (parentObj->find(parentName) == parentObj->end())
            (*parentObj)[parentName] = RayTracer::NodePtr(new RayTracer::Node{RayTracer::Object{}});
        parentObj = &std::get<RayTracer::Object>((*parentObj)[parentName]->value);
        (*parentObj)[childName] = RayTracer::NodePtr(new RayTracer::Node{value});
    }
    else
        obj[name] = RayTracer::NodePtr(new RayTracer::Node{value});
}

/**
 * @brief Inserts a vector of objects into the object structure, creating nested objects as needed.
 * @param obj The object to insert the vector into
 * @param name The name of the vector
 * @param s The libconfig setting (array or list of groups) to extract the vector from
 * @return void
 */
void RayTracer::Parser::insert_vector_object(RayTracer::Object &obj, const std::string &name, const libconfig::Setting &s)
{
    RayTracer::VectorObject vectorObj;

    for (int i = 0; i < s.getLength(); ++i)
    {
        const libconfig::Setting &groupElement = s[i];
        if (groupElement.isGroup())
        {
            RayTracer::Object groupObj;
            parseSettings(groupElement, groupObj, "");
            vectorObj.push_back(std::move(groupObj));
        }
    }

    size_t lastDot = name.rfind('.');

    if (lastDot != std::string::npos)
    {
        std::string parentName = name.substr(0, lastDot);
        std::string childName = name.substr(lastDot + 1);
        RayTracer::Object *parentObj = &obj;
        size_t dotPos = parentName.find('.');

        while (dotPos != std::string::npos)
        {
            std::string currentPart = parentName.substr(0, dotPos);
            if (parentObj->find(currentPart) == parentObj->end())
            {
                (*parentObj)[currentPart] = RayTracer::NodePtr(new RayTracer::Node{RayTracer::Object{}});
            }
            parentObj = &std::get<RayTracer::Object>((*parentObj)[currentPart]->value);
            parentName = parentName.substr(dotPos + 1);
            dotPos = parentName.find('.');
        }
        if (parentObj->find(parentName) == parentObj->end())
            (*parentObj)[parentName] = RayTracer::NodePtr(new RayTracer::Node{RayTracer::Object{}});
        parentObj = &std::get<RayTracer::Object>((*parentObj)[parentName]->value);
        (*parentObj)[childName] = RayTracer::NodePtr(new RayTracer::Node{std::move(vectorObj)});
    }
    else
        obj[name] = RayTracer::NodePtr(new RayTracer::Node{std::move(vectorObj)});
}

/**
 * @brief Parses the settings from a libconfig setting and inserts them into the object structure.
 * @param setting The libconfig setting to parse
 * @param result The object to insert the parsed values into
 * @param prefix The prefix for the values' names
 * @return void
 */
void RayTracer::Parser::parseSettings(const libconfig::Setting &setting, RayTracer::Object &result, const std::string prefix)
{
    for (int i = 0; i < setting.getLength(); ++i)
    {
        const libconfig::Setting &s = setting[i];

        if (s.isGroup())
            parseSettings(s, result, prefix + s.getName() + ".");
        else if (s.isArray() || s.isList())
        {
            if (s.getLength() > 0 && s[0].isGroup())
                insert_vector_object(result, prefix + s.getName(), s);
            else
                for (int j = 0; j < s.getLength(); ++j)
                {
                    const libconfig::Setting &arrayElement = s[j];
                    insert_value(result, prefix + s.getName() + "[" + std::to_string(j) + "].", arrayElement);
                }
        }
        else
            insert_value(result, prefix, s);
    }
}
