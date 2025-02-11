#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Utils/Helpers/CompilerSpecific.h"

namespace DummyEngine {

    enum class ScriptFieldType {
        None = 0,
        Float,
        Double,
        Bool,
        String,
        S8,
        S16,
        S32,
        S64,
        U8,
        U16,
        U32,
        U64,
        Vec2,
        Vec3,
        Vec4,
        Entity,
    };
    struct ScriptClassField {
        ScriptFieldType Type;
        U32             Offset;
    };

    class Script {
    private:
        using Iterator = std::unordered_map<std::string, ScriptClassField>::const_iterator;

    public:
        class Field {
        public:
            Field() = default;
            Field(ScriptFieldType type, void* ptr);
            template <typename T> T&       Get();
            template <typename T> const T& Get() const;
            ScriptFieldType                GetType() const { return type_; }
            void*                          Get() const { return data_; }

        private:
            ScriptFieldType type_;
            void*           data_;
        };
        class FieldIterator {
        public:
            bool                                                        operator==(const FieldIterator& other) const;
            bool                                                        operator!=(const FieldIterator& other) const;
            FieldIterator&                                              operator++();
            FieldIterator                                               operator++(int);
            std::pair<std::reference_wrapper<const std::string>, Field> operator*() const;

        private:
            friend class Script;
            FieldIterator(Script* owner, Iterator it);

            Iterator iterator_;
            Script*  script_;
        };

        Script()                         = default;
        virtual ~Script()                = default;
        Script(const Script&)            = delete;
        Script(Script&&)                 = delete;
        Script& operator=(const Script&) = delete;
        Script& operator=(Script&&)      = delete;

        virtual void OnAttach() {}
        virtual void OnRuntimeStart() {}
        virtual void OnUpdate(float) {}
        virtual void OnRender() {}
        virtual void OnRuntimeStop() {}
        virtual void OnDetach() {}

        FieldIterator begin();  // NOLINT
        FieldIterator end();    // NOLINT

        template <typename T> T& GetField(const std::string& name);
        ScriptFieldType          GetFieldType(const std::string& name) const;
        bool                     HasField(const std::string& name) const;
        void                     AttachToScene(WeakRef<Scene> scene, Entity entity);
        bool                     AttachedToScene() const;
        Ref<Scene>               GetScene() const;

    protected:
        virtual const std::unordered_map<std::string, ScriptClassField>& GetClassFields() const = 0;

        template <typename T> T&   Add(const T& t);
        template <typename T> T&   Get();
        template <typename T> bool Has();
        template <typename T> void Remove();

        WeakRef<Scene> m_Scene;
        Entity         m_Entity;
    };
    std::string                           ScriptFieldTypeToString(ScriptFieldType type);
    template <typename T> ScriptFieldType TypeToScriptFieldType();

    // clang-format off
    template <typename T, typename U> constexpr U32 OffsetOf(U T::* member) {
        return (size_t)&((T*)nullptr->*member);
    }
    // clang-format on

    template <typename T> T& Script::Field::Get() {
        DE_ASSERT(type_ == TypeToScriptFieldType<T>(),
                  "Wrong field type {} expected {}",
                  ScriptFieldTypeToString(TypeToScriptFieldType<T>()),
                  ScriptFieldTypeToString(type_));
        return *(T*)data_;
    }
    template <typename T> const T& Script::Field::Get() const {
        DE_ASSERT(type_ == TypeToScriptFieldType<T>(),
                  "Wrong field type {} expected {}",
                  ScriptFieldTypeToString(TypeToScriptFieldType<T>()),
                  ScriptFieldTypeToString(type_));
        return *(T*)data_;
    }
    template <typename T> T& Script::GetField(const std::string& name) {
        DE_ASSERT(GetClassFields().contains(name), "Wrong field name(", name, ")");
        DE_ASSERT(GetClassFields().at(name).Type == TypeToScriptFieldType<T>(),
                  "Wrong field type {} expected {}",
                  ScriptFieldTypeToString(TypeToScriptFieldType<T>()),
                  ScriptFieldTypeToString(GetClassFields().at(name).Type));
        return *(T*)((char*)this + GetClassFields().at(name).Offset);
    }
    template <typename T> T& Script::Add(const T& t) {
        DE_ASSERT(m_Entity.Valid(), "Using invalid entity in script");
        return m_Entity.Add<T>(t);
    }
    template <typename T> T& Script::Get() {
        DE_ASSERT(m_Entity.Valid(), "Using invalid entity in script");
        return m_Entity.Get<T>();
    }
    template <typename T> bool Script::Has() {
        DE_ASSERT(m_Entity.Valid(), "Using invalid entity in script");
        return m_Entity.Has<T>();
    }
    template <typename T> void Script::Remove() {
        DE_ASSERT(m_Entity.Valid(), "Using invalid entity in script");
        m_Entity.Remove<T>();
    }

#define FIELD(field_name, field)                                                          \
    {                                                                                     \
        field_name, {                                                                     \
            TypeToScriptFieldType<decltype(field)>(), OffsetOf(&CurrentScriptType::field) \
        }                                                                                 \
    }

#define SCRIPT(type)                                                                                   \
    static const std::unordered_map<std::string, ScriptClassField> s_ClassFields;                      \
                                                                                                       \
protected:                                                                                             \
    virtual const std::unordered_map<std::string, ScriptClassField>& GetClassFields() const override { \
        return s_ClassFields;                                                                          \
    }

#define SCRIPT_BASE(type, ...)                                                                   \
    using CurrentScriptType                                                     = type;          \
    const std::unordered_map<std::string, ScriptClassField> type::s_ClassFields = {__VA_ARGS__}; \
    DE_SCRIPT_API Script*                                   type##Create() {                     \
        return new type();                                     \
    }                                                                                            \
    DE_SCRIPT_API void type##Delete(Script* script) {                                            \
        delete script;                                                                           \
    }

}  // namespace DummyEngine
