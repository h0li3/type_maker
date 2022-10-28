#ifndef TYPE_MAKER_MAKER_H_
#define TYPE_MAKER_MAKER_H_

#include <cstddef>
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>


struct TypeInfo
{
    size_t length;
    int build_number;
    std::string name;
    std::map<std::string, size_t> fields;
};

class TypeMaker
{
public:
    static std::map<std::string, std::vector<TypeInfo>>& GetAllTypes()
    {
        static std::map<std::string, std::vector<TypeInfo>> types;
        return types;
    }

    static void AddTypeInfo(std::string&& name, TypeInfo&& t)
    {
        auto& types = GetAllTypes();
        auto iter = types.find(name);
        if (iter == types.end()) {
            std::vector<TypeInfo> type;
            type.push_back(std::move(t));
            types[std::move(name)] = std::move(type);
        }
        else {
            iter->second.push_back(t);
        }
    }

    static void GenerateTypeDesc(const std::string& type_name, const std::vector<TypeInfo>& info)
    {
        // type desc structure def
        std::vector<std::string> names;
        for (const auto& i : info) {
            for (auto& f : i.fields) {
                bool found = false;
                for (auto& n : names) {
                    if (n == f.first) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    names.push_back(f.first);
                }
            }
        }
        std::cout
            << "#pragma once\n"
            << "#include <stddef.h>\n\n"
            << "struct TypeDesc_" << type_name << " {\n"
            << "    int build_number;\n"
            << "    size_t size;\n";
        for (auto& name : names) {
            std::cout
                << "    size_t off_" << name << ";\n";
        }
        std::cout << "};\n";

        // type offsets variables
        std::cout
            << "static const size_t neg1 = (size_t)-1;\n"
            << "static const struct TypeDesc_" << type_name << " __gtd_" << type_name << "[" << info.size() << "] = {\n";
        for (const auto& i : info) {
            size_t off = -1;
            std::cout << "    { " << i.build_number << ", " << i.length;
            for (auto& name : names) {
                auto iter = i.fields.find(name);
                if (iter != i.fields.end()) {
                    std::cout << ", " << iter->second;
                }
                else {
                    std::cout << ", neg1";
                }
            }
            std::cout << " },\n";
        }
        std::cout << "};\n";
    }

    static void GenerateSource()
    {
        for (auto& t : GetAllTypes()) {
            if (t.second.empty() == false) {
                GenerateTypeDesc(t.first, t.second);
            }
        }
    }
};

template<typename T, typename F>
static constexpr size_t OffsetOf(F T::*M)
{
    return reinterpret_cast<size_t>(&(reinterpret_cast<T*>(0)->*M));
}

template<typename T, typename...F>
class RegisterStruct {
public:
    RegisterStruct(const char* name, int build_number, F T::*...M)
        : length(sizeof(T)),
        offsets{OffsetOf(M)...},
        type_name{name},
        build_number(build_number)
    {
    }

    void commit_fields_name(const char* type, const char* fields_name)
    {
        const char* p = fields_name;

        while (*p) {
            if (*p == '&') {
                ++p;
                while (*p && *p != ':') {
                    ++p;
                }
                p += 2;

                int len = 0;
                while (p[len] && p[len] != ' ' && p[len] != ',') {
                    len++;
                }
                if (len > 0) {
                    names.emplace_back(std::string(p, len));
                    p += len;
                }
            }
            ++p;
        }
    }

    void make_type()
    {
        TypeInfo info
        {
            length,
            build_number,
            type_name
        };

        for (size_t i = 0; i < offsets.size(); ++i) {
            info.fields[names[i]] = offsets[i];
        }
        
        TypeMaker::AddTypeInfo(std::move(type_name), std::move(info));
    }

private:
    size_t length;
    int build_number;
    std::string type_name;
    std::vector<size_t> offsets;
    std::vector<std::string> names;
};


#define register_type(name, type, build_number, ...) \
    RegisterStruct __struct_desc_##type##__(name, build_number, __VA_ARGS__);\
    __struct_desc_##type##__.commit_fields_name(#type, #__VA_ARGS__);\
    __struct_desc_##type##__.make_type();

#endif // TYPE_MAKER_MAKER_H_