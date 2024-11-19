#include "DummyEngine/Core/Scene/SceneHierarchy.h"

#include "DummyEngine/Core/Scene/Components.h"

namespace DE {
    bool SceneHierarchy::Node::Valid() const {
        return m_ID != UINT32_MAX && m_Owner && !std::holds_alternative<std::monostate>(m_Owner->m_Nodes[m_ID].node);
    }
    bool SceneHierarchy::Node::IsEntity() const {
        return m_ID != UINT32_MAX && m_Owner && std::holds_alternative<NodeData::EntityNode>(m_Owner->m_Nodes[m_ID].node);
    }
    bool SceneHierarchy::Node::IsFolder() const {
        return m_ID != UINT32_MAX && m_Owner && std::holds_alternative<NodeData::FolderNode>(m_Owner->m_Nodes[m_ID].node);
    }
    bool SceneHierarchy::Node::IsAnsestorOf(const Node& child) const {
        if (!Valid() || !child.Valid() || m_Owner != child.m_Owner) {
            return false;
        }
        return m_Owner->IsAnsestor(m_ID, child.m_ID);
    }
    U32 SceneHierarchy::Node::GetID() const {
        return m_ID;
    }
    SceneHierarchy::Node SceneHierarchy::Node::GetParent() const {
        DE_ASSERT(m_ID != UINT32_MAX && m_Owner, "Use of GetParent on invalid SceneHierarchy::Node");
        return Node(m_Owner, m_Owner->m_Nodes[m_ID].parent);
    }

    bool SceneHierarchy::Node::Attach(Node other) {
        DE_ASSERT(m_ID != UINT32_MAX && m_Owner, "Use of Attach on invalid SceneHierarchy::Node");
        return m_Owner->Attach(*this, other);
    }
    SceneHierarchy::Node SceneHierarchy::Node::AddEntity(Entity entity) {
        DE_ASSERT(IsFolder(), "Use of AddEntity on invalid SceneHierarchy::Node");
        auto ent = m_Owner->AddEntity(entity);
        m_Owner->Attach(*this, ent);
        return ent;
    }
    SceneHierarchy::Node SceneHierarchy::Node::AddFolder(const std::string& name) {
        DE_ASSERT(IsFolder(), "Use of AddFolder on invalid SceneHierarchy::Node");
        auto folder = m_Owner->AddFolder(name);
        m_Owner->Attach(*this, folder);
        return folder;
    }
    void SceneHierarchy::Node::Delete() {
        DE_ASSERT(m_ID != UINT32_MAX && m_Owner, "Use of Delete on invalid SceneHierarchy::Node");
        m_Owner->DeleteNode(*this);
        *this = Node();
    }

    Entity SceneHierarchy::Node::GetEntity() const {
        DE_ASSERT(IsEntity(), "Use of GetEntity on non entity node");
        auto& entity = std::get<NodeData::EntityNode>(m_Owner->m_Nodes[m_ID].node);
        return entity.entity;
    }
    std::vector<SceneHierarchy::Node> SceneHierarchy::Node::GetChilds() const {
        DE_ASSERT(IsFolder(), "Use of GetChilds on non folder node");
        std::vector<Node> res;
        auto&             folder = std::get<NodeData::FolderNode>(m_Owner->m_Nodes[m_ID].node);
        for (auto child : folder.childs) {
            res.push_back(Node(m_Owner, child));
        }
        return res;
    }
    std::string& SceneHierarchy::Node::GetName() {
        DE_ASSERT(IsFolder(), "Use of GetName on non folder node");
        auto& folder = std::get<NodeData::FolderNode>(m_Owner->m_Nodes[m_ID].node);
        return folder.name;
    }
    bool SceneHierarchy::Node::operator==(const Node& other) const {
        return m_ID == other.m_ID && m_Owner == other.m_Owner;
    }
    bool SceneHierarchy::Node::operator!=(const Node& other) const {
        return m_ID != other.m_ID || m_Owner != other.m_Owner;
    }

    SceneHierarchy::Node::Node(SceneHierarchy* owner, U32 id) : m_Owner(owner), m_ID(id) {}

    SceneHierarchy::SceneHierarchy(const std::string& name) {
        m_Nodes.emplace_back(name);
    }

    SceneHierarchy::Node SceneHierarchy::GetRoot() {
        return Node(this, 0);
    }
    SceneHierarchy::Node SceneHierarchy::AddFolder(const std::string& name) {
        U32 id;
        if (m_AvailableNodes.empty()) {
            id = m_Nodes.size();
            m_Nodes.emplace_back(name, 0);
        } else {
            id = m_AvailableNodes.front();
            m_AvailableNodes.pop();
            m_Nodes[id] = NodeData(name, 0);
        }
        std::get<NodeData::FolderNode>(m_Nodes[0].node).childs.push_back(id);
        return Node(this, id);
    }
    SceneHierarchy::Node SceneHierarchy::AddEntity(Entity entity) {
        U32 id;
        if (m_AvailableNodes.empty()) {
            id = m_Nodes.size();
            m_Nodes.emplace_back(entity, 0);
        } else {
            id = m_AvailableNodes.front();
            m_AvailableNodes.pop();
            m_Nodes[id] = NodeData(entity, 0);
        }
        std::get<NodeData::FolderNode>(m_Nodes[0].node).childs.push_back(id);
        return Node(this, id);
    }
    void SceneHierarchy::DeleteNode(Node node) {
        DE_ASSERT(node.m_ID < m_Nodes.size(), "Use of DeleteNode on invalid SceneHierarchy::Node");
        DE_ASSERT(node.m_ID != 0, "Use of DeleteNode on root SceneHierarchy::Node");
        auto& parent = std::get<NodeData::FolderNode>(m_Nodes[m_Nodes[node.m_ID].parent].node);
        for (auto it = parent.childs.begin(); it != parent.childs.end(); ++it) {
            if (*it == node.m_ID) {
                parent.childs.erase(it);
                break;
            }
        }
        m_Nodes[node.m_ID] = NodeData();
        m_AvailableNodes.push(node.m_ID);
    }
    bool SceneHierarchy::Attach(Node parent, Node child) {
        DE_ASSERT(child.m_ID < m_Nodes.size(), "Use of Attach on invalid child SceneHierarchy::Node");
        DE_ASSERT(parent.m_ID < m_Nodes.size(), "Use of Attach on invalid parent SceneHierarchy::Node");
        U32 new_parent_id = parent.m_ID;
        U32 old_parent_id = m_Nodes[child.m_ID].parent;
        U32 child_id      = child.m_ID;
        if (new_parent_id == old_parent_id || !parent.IsFolder() || IsAnsestor(child_id, new_parent_id)) {
            return false;
        }
        auto& old_parent = std::get<NodeData::FolderNode>(m_Nodes[old_parent_id].node);
        auto& new_parent = std::get<NodeData::FolderNode>(m_Nodes[new_parent_id].node);
        for (auto it = old_parent.childs.begin(); it != old_parent.childs.end(); ++it) {
            if (*it == child.m_ID) {
                old_parent.childs.erase(it);
                break;
            }
        }
        m_Nodes[child_id].parent = new_parent_id;
        new_parent.childs.push_back(child_id);
        return true;
    }

    bool SceneHierarchy::IsAnsestor(U32 parent, U32 child) const {
        do {
            if (m_Nodes[child].parent == parent) {
                return true;
            }
            child = m_Nodes[child].parent;
        } while (child != UINT32_MAX);
        return false;
    }

    SceneHierarchy::NodeData::NodeData(Entity entity, U32 parent) : node(EntityNode(entity)), parent(parent) {}
    SceneHierarchy::NodeData::NodeData(const std::string& name, U32 parent) : node(FolderNode(name)), parent(parent) {}
    SceneHierarchy::NodeData::EntityNode::EntityNode(Entity ent) : entity(ent) {}
    SceneHierarchy::NodeData::FolderNode::FolderNode(const std::string& name) : name(name) {}
}  // namespace DE