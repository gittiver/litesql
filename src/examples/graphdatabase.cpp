#include "graphdatabase.hpp"
namespace graph {
using namespace litesql;
NodePropertyRelationNodeProperties::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : property(NodePropertyRelationNodeProperties::Property), node(NodePropertyRelationNodeProperties::Node) {
    switch(rec.size()) {
    case 2:
        property = rec[1];
    case 1:
        node = rec[0];
    }
}
const std::string NodePropertyRelationNodeProperties::table__("Node_Property_NodeProperties");
const litesql::FieldType NodePropertyRelationNodeProperties::Node("Node1_",A_field_type_integer,table__);
const litesql::FieldType NodePropertyRelationNodeProperties::Property("Property2_",A_field_type_integer,table__);
void NodePropertyRelationNodeProperties::link(const litesql::Database& db, const graph::Node& o0, const graph::Property& o1) {
    Record values;
    Record fields;
    fields.push_back(Node.name());
    values.push_back(o0.id);
    fields.push_back(Property.name());
    values.push_back(o1.id);
    db.insert(table__, values, fields);
}
void NodePropertyRelationNodeProperties::unlink(const litesql::Database& db, const graph::Node& o0, const graph::Property& o1) {
    db.delete_(table__, (Node == o0.id && Property == o1.id));
}
void NodePropertyRelationNodeProperties::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<NodePropertyRelationNodeProperties::Row> NodePropertyRelationNodeProperties::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Node.fullName());
    sel.result(Property.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<NodePropertyRelationNodeProperties::Row>(db, sel);
}
template <> litesql::DataSource<graph::Node> NodePropertyRelationNodeProperties::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Node.fullName());
    sel.where(srcExpr);
    return DataSource<graph::Node>(db, graph::Node::Id.in(sel) && expr);
}
template <> litesql::DataSource<graph::Property> NodePropertyRelationNodeProperties::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Property.fullName());
    sel.where(srcExpr);
    return DataSource<graph::Property>(db, graph::Property::Id.in(sel) && expr);
}
NodeTagRelationNodeTags::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : type(NodeTagRelationNodeTags::Type), tag(NodeTagRelationNodeTags::Tag), node(NodeTagRelationNodeTags::Node) {
    switch(rec.size()) {
    case 3:
        type = rec[2];
    case 2:
        tag = rec[1];
    case 1:
        node = rec[0];
    }
}
const std::string NodeTagRelationNodeTags::table__("Node_Tag_NodeTags");
const litesql::FieldType NodeTagRelationNodeTags::Node("Node1_",A_field_type_integer,table__);
const litesql::FieldType NodeTagRelationNodeTags::Tag("Tag2_",A_field_type_integer,table__);
const litesql::FieldType NodeTagRelationNodeTags::Type("type_",A_field_type_integer,table__);
void NodeTagRelationNodeTags::link(const litesql::Database& db, const graph::Node& o0, const graph::Tag& o1, int type) {
    Record values;
    Record fields;
    fields.push_back(Node.name());
    values.push_back(o0.id);
    fields.push_back(Tag.name());
    values.push_back(o1.id);
    fields.push_back(Type.name());
    values.push_back(toString(type));
    db.insert(table__, values, fields);
}
void NodeTagRelationNodeTags::unlink(const litesql::Database& db, const graph::Node& o0, const graph::Tag& o1, int type) {
    db.delete_(table__, (Node == o0.id && Tag == o1.id && (Type == type)));
}
void NodeTagRelationNodeTags::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<NodeTagRelationNodeTags::Row> NodeTagRelationNodeTags::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Node.fullName());
    sel.result(Tag.fullName());
    sel.result(Type.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<NodeTagRelationNodeTags::Row>(db, sel);
}
template <> litesql::DataSource<graph::Node> NodeTagRelationNodeTags::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Node.fullName());
    sel.where(srcExpr);
    return DataSource<graph::Node>(db, graph::Node::Id.in(sel) && expr);
}
template <> litesql::DataSource<graph::Tag> NodeTagRelationNodeTags::get(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Tag.fullName());
    sel.where(srcExpr);
    return DataSource<graph::Tag>(db, graph::Tag::Id.in(sel) && expr);
}
NodeNodeRelationEdges::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : type(NodeNodeRelationEdges::Type), node2(NodeNodeRelationEdges::Node2), node1(NodeNodeRelationEdges::Node1) {
    switch(rec.size()) {
    case 3:
        type = rec[2];
    case 2:
        node2 = rec[1];
    case 1:
        node1 = rec[0];
    }
}
const std::string NodeNodeRelationEdges::table__("Node_Node_Edges");
const litesql::FieldType NodeNodeRelationEdges::Node1("Node1_",A_field_type_integer,table__);
const litesql::FieldType NodeNodeRelationEdges::Node2("Node2_",A_field_type_integer,table__);
const litesql::FieldType NodeNodeRelationEdges::Type("type_",A_field_type_integer,table__);
void NodeNodeRelationEdges::link(const litesql::Database& db, const graph::Node& o0, const graph::Node& o1, int type) {
    Record values;
    Record fields;
    fields.push_back(Node1.name());
    values.push_back(o0.id);
    fields.push_back(Node2.name());
    values.push_back(o1.id);
    fields.push_back(Type.name());
    values.push_back(toString(type));
    db.insert(table__, values, fields);
    fields.clear();
    values.clear();
    fields.push_back(Node1.name());
    values.push_back(o1.id);
    fields.push_back(Node2.name());
    values.push_back(o0.id);
    fields.push_back(Type.name());
    values.push_back(toString(type));
    db.insert(table__, values, fields);
}
void NodeNodeRelationEdges::unlink(const litesql::Database& db, const graph::Node& o0, const graph::Node& o1, int type) {
    db.delete_(table__, (Node1 == o0.id && Node2 == o1.id && (Type == type)));
    db.delete_(table__, (Node1 == o1.id && Node2 == o0.id && (Type == type)));
}
void NodeNodeRelationEdges::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<NodeNodeRelationEdges::Row> NodeNodeRelationEdges::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Node1.fullName());
    sel.result(Node2.fullName());
    sel.result(Type.fullName());
    sel.source(table__);
    sel.where(expr);
    return DataSource<NodeNodeRelationEdges::Row>(db, sel);
}
litesql::DataSource<graph::Node> NodeNodeRelationEdges::getNode1(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Node1.fullName());
    sel.where(srcExpr);
    return DataSource<graph::Node>(db, graph::Node::Id.in(sel) && expr);
}
litesql::DataSource<graph::Node> NodeNodeRelationEdges::getNode2(const litesql::Database& db, const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    SelectQuery sel;
    sel.source(table__);
    sel.result(Node2.fullName());
    sel.where(srcExpr);
    return DataSource<graph::Node>(db, graph::Node::Id.in(sel) && expr);
}
const litesql::FieldType Node::Own::Id("id_",A_field_type_integer,"Node_");
Node::PropertiesHandle::PropertiesHandle(const Node& owner)
         : litesql::RelationHandle<Node>(owner) {
}
void Node::PropertiesHandle::link(const Property& o0) {
    NodePropertyRelationNodeProperties::link(owner->getDatabase(), *owner, o0);
}
void Node::PropertiesHandle::unlink(const Property& o0) {
    NodePropertyRelationNodeProperties::unlink(owner->getDatabase(), *owner, o0);
}
void Node::PropertiesHandle::del(const litesql::Expr& expr) {
    NodePropertyRelationNodeProperties::del(owner->getDatabase(), expr && NodePropertyRelationNodeProperties::Node == owner->id);
}
litesql::DataSource<Property> Node::PropertiesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return NodePropertyRelationNodeProperties::get<Property>(owner->getDatabase(), expr, (NodePropertyRelationNodeProperties::Node == owner->id) && srcExpr);
}
litesql::DataSource<NodePropertyRelationNodeProperties::Row> Node::PropertiesHandle::getRows(const litesql::Expr& expr) {
    return NodePropertyRelationNodeProperties::getRows(owner->getDatabase(), expr && (NodePropertyRelationNodeProperties::Node == owner->id));
}
Node::TagsHandle::TagsHandle(const Node& owner)
         : litesql::RelationHandle<Node>(owner) {
}
void Node::TagsHandle::link(const Tag& o0, int type) {
    NodeTagRelationNodeTags::link(owner->getDatabase(), *owner, o0, type);
}
void Node::TagsHandle::unlink(const Tag& o0, int type) {
    NodeTagRelationNodeTags::unlink(owner->getDatabase(), *owner, o0, type);
}
void Node::TagsHandle::del(const litesql::Expr& expr) {
    NodeTagRelationNodeTags::del(owner->getDatabase(), expr && NodeTagRelationNodeTags::Node == owner->id);
}
litesql::DataSource<Tag> Node::TagsHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return NodeTagRelationNodeTags::get<Tag>(owner->getDatabase(), expr, (NodeTagRelationNodeTags::Node == owner->id) && srcExpr);
}
litesql::DataSource<NodeTagRelationNodeTags::Row> Node::TagsHandle::getRows(const litesql::Expr& expr) {
    return NodeTagRelationNodeTags::getRows(owner->getDatabase(), expr && (NodeTagRelationNodeTags::Node == owner->id));
}
Node::EdgesHandle::EdgesHandle(const Node& owner)
         : litesql::RelationHandle<Node>(owner) {
}
void Node::EdgesHandle::link(const Node& o0, int type) {
    NodeNodeRelationEdges::link(owner->getDatabase(), *owner, o0, type);
}
void Node::EdgesHandle::unlink(const Node& o0, int type) {
    NodeNodeRelationEdges::unlink(owner->getDatabase(), *owner, o0, type);
}
void Node::EdgesHandle::del(const litesql::Expr& expr) {
    NodeNodeRelationEdges::del(owner->getDatabase(), expr && NodeNodeRelationEdges::Node1 == owner->id);
}
litesql::DataSource<Node> Node::EdgesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return NodeNodeRelationEdges::getNode2(owner->getDatabase(), expr, (NodeNodeRelationEdges::Node1 == owner->id) && srcExpr);
}
litesql::DataSource<NodeNodeRelationEdges::Row> Node::EdgesHandle::getRows(const litesql::Expr& expr) {
    return NodeNodeRelationEdges::getRows(owner->getDatabase(), expr && (NodeNodeRelationEdges::Node1 == owner->id));
}
const std::string Node::type__("Node");
const std::string Node::table__("Node_");
const std::string Node::sequence__("Node_seq");
const litesql::FieldType Node::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Node::Type("type_",A_field_type_string,table__);
const litesql::FieldType Node::Name("name_",A_field_type_string,table__);
void Node::initValues() {
}
void Node::defaults() {
    id = 0;
}
Node::Node(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name) {
    defaults();
}
Node::Node(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name) {
    defaults();
    size_t size = (rec.size() > 3) ? 3 : rec.size();
    switch(size) {
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Node::Node(const Node& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name) {
}
const Node& Node::operator=(const Node& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Node::PropertiesHandle Node::properties() {
    return Node::PropertiesHandle(*this);
}
Node::TagsHandle Node::tags() {
    return Node::TagsHandle(*this);
}
Node::EdgesHandle Node::edges() {
    return Node::EdgesHandle(*this);
}
std::string Node::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Node::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Node::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
}
void Node::addIDUpdates(Updates& updates) {
}
void Node::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
}
void Node::delRecord() {
    deleteFromTable(table__, id);
}
void Node::delRelations() {
    NodePropertyRelationNodeProperties::del(*db, (NodePropertyRelationNodeProperties::Node == id));
    NodeTagRelationNodeTags::del(*db, (NodeTagRelationNodeTags::Node == id));
    NodeNodeRelationEdges::del(*db, (NodeNodeRelationEdges::Node1 == id) || (NodeNodeRelationEdges::Node2 == id));
}
void Node::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Node::del() {
    if (!typeIsCorrect()) {
        std::unique_ptr<Node> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Node::typeIsCorrect() const {
    return type == type__;
}
std::unique_ptr<Node> Node::upcast() const {
    if (type == ColoredNode::type__)
        return unique_ptr<Node>(new ColoredNode(select<ColoredNode>(*db, Id == id).one()));
    return unique_ptr<Node>(new Node(*this));
}
std::unique_ptr<Node> Node::upcastCopy() const {
    Node* np = NULL;
    if (type == "ColoredNode")
        np = new ColoredNode(*db);
    np->id = id;
    np->type = type;
    np->name = name;
    np->inDatabase = inDatabase;
    return unique_ptr<Node>(np);
}
std::ostream & operator<<(std::ostream& os, Node o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Tag::Own::Id("id_",A_field_type_integer,"Tag_");
Tag::NodesHandle::NodesHandle(const Tag& owner)
         : litesql::RelationHandle<Tag>(owner) {
}
void Tag::NodesHandle::link(const Node& o0, int type) {
    NodeTagRelationNodeTags::link(owner->getDatabase(), o0, *owner, type);
}
void Tag::NodesHandle::unlink(const Node& o0, int type) {
    NodeTagRelationNodeTags::unlink(owner->getDatabase(), o0, *owner, type);
}
void Tag::NodesHandle::del(const litesql::Expr& expr) {
    NodeTagRelationNodeTags::del(owner->getDatabase(), expr && NodeTagRelationNodeTags::Tag == owner->id);
}
litesql::DataSource<Node> Tag::NodesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return NodeTagRelationNodeTags::get<Node>(owner->getDatabase(), expr, (NodeTagRelationNodeTags::Tag == owner->id) && srcExpr);
}
litesql::DataSource<NodeTagRelationNodeTags::Row> Tag::NodesHandle::getRows(const litesql::Expr& expr) {
    return NodeTagRelationNodeTags::getRows(owner->getDatabase(), expr && (NodeTagRelationNodeTags::Tag == owner->id));
}
const std::string Tag::type__("Tag");
const std::string Tag::table__("Tag_");
const std::string Tag::sequence__("Tag_seq");
const litesql::FieldType Tag::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Tag::Type("type_",A_field_type_string,table__);
const litesql::FieldType Tag::Name("name_",A_field_type_string,table__);
void Tag::initValues() {
}
void Tag::defaults() {
    id = 0;
}
Tag::Tag(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name) {
    defaults();
}
Tag::Tag(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name) {
    defaults();
    size_t size = (rec.size() > 3) ? 3 : rec.size();
    switch(size) {
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Tag::Tag(const Tag& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name) {
}
const Tag& Tag::operator=(const Tag& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Tag::NodesHandle Tag::nodes() {
    return Tag::NodesHandle(*this);
}
std::string Tag::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Tag::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Tag::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
}
void Tag::addIDUpdates(Updates& updates) {
}
void Tag::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
}
void Tag::delRecord() {
    deleteFromTable(table__, id);
}
void Tag::delRelations() {
    NodeTagRelationNodeTags::del(*db, (NodeTagRelationNodeTags::Tag == id));
}
void Tag::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Tag::del() {
    if (!typeIsCorrect()) {
        std::unique_ptr<Tag> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Tag::typeIsCorrect() const {
    return type == type__;
}
std::unique_ptr<Tag> Tag::upcast() const {
    return unique_ptr<Tag>(new Tag(*this));
}
std::unique_ptr<Tag> Tag::upcastCopy() const {
    Tag* np = new Tag(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->inDatabase = inDatabase;
    return unique_ptr<Tag>(np);
}
std::ostream & operator<<(std::ostream& os, Tag o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType Property::Own::Id("id_",A_field_type_integer,"Property_");
Property::NodesHandle::NodesHandle(const Property& owner)
         : litesql::RelationHandle<Property>(owner) {
}
void Property::NodesHandle::link(const Node& o0) {
    NodePropertyRelationNodeProperties::link(owner->getDatabase(), o0, *owner);
}
void Property::NodesHandle::unlink(const Node& o0) {
    NodePropertyRelationNodeProperties::unlink(owner->getDatabase(), o0, *owner);
}
void Property::NodesHandle::del(const litesql::Expr& expr) {
    NodePropertyRelationNodeProperties::del(owner->getDatabase(), expr && NodePropertyRelationNodeProperties::Property == owner->id);
}
litesql::DataSource<Node> Property::NodesHandle::get(const litesql::Expr& expr, const litesql::Expr& srcExpr) {
    return NodePropertyRelationNodeProperties::get<Node>(owner->getDatabase(), expr, (NodePropertyRelationNodeProperties::Property == owner->id) && srcExpr);
}
litesql::DataSource<NodePropertyRelationNodeProperties::Row> Property::NodesHandle::getRows(const litesql::Expr& expr) {
    return NodePropertyRelationNodeProperties::getRows(owner->getDatabase(), expr && (NodePropertyRelationNodeProperties::Property == owner->id));
}
const std::string Property::type__("Property");
const std::string Property::table__("Property_");
const std::string Property::sequence__("Property_seq");
const litesql::FieldType Property::Id("id_",A_field_type_integer,table__);
const litesql::FieldType Property::Type("type_",A_field_type_string,table__);
const litesql::FieldType Property::Name("name_",A_field_type_string,table__);
const litesql::FieldType Property::Value("value_",A_field_type_string,table__);
void Property::initValues() {
}
void Property::defaults() {
    id = 0;
}
Property::Property(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), value(Value) {
    defaults();
}
Property::Property(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), value(Value) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: value = convert<const std::string&, std::string>(rec[3]);
        value.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Property::Property(const Property& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), value(obj.value) {
}
const Property& Property::operator=(const Property& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        value = obj.value;
    }
    litesql::Persistent::operator=(obj);
    return *this;
}
Property::NodesHandle Property::nodes() {
    return Property::NodesHandle(*this);
}
std::string Property::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back(id.name());
    values.push_back(id);
    id.setModified(false);
    fields.push_back(type.name());
    values.push_back(type);
    type.setModified(false);
    fields.push_back(name.name());
    values.push_back(name);
    name.setModified(false);
    fields.push_back(value.name());
    values.push_back(value);
    value.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return litesql::Persistent::insert(tables, fieldRecs, valueRecs, sequence__);
}
void Property::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void Property::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
    updateField(updates, table__, type);
    updateField(updates, table__, name);
    updateField(updates, table__, value);
}
void Property::addIDUpdates(Updates& updates) {
}
void Property::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Value);
}
void Property::delRecord() {
    deleteFromTable(table__, id);
}
void Property::delRelations() {
    NodePropertyRelationNodeProperties::del(*db, (NodePropertyRelationNodeProperties::Property == id));
}
void Property::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void Property::del() {
    if (!typeIsCorrect()) {
        std::unique_ptr<Property> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool Property::typeIsCorrect() const {
    return type == type__;
}
std::unique_ptr<Property> Property::upcast() const {
    return unique_ptr<Property>(new Property(*this));
}
std::unique_ptr<Property> Property::upcastCopy() const {
    Property* np = new Property(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->value = value;
    np->inDatabase = inDatabase;
    return unique_ptr<Property>(np);
}
std::ostream & operator<<(std::ostream& os, Property o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.value.name() << " = " << o.value << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
const litesql::FieldType ColoredNode::Own::Id("id_",A_field_type_integer,"ColoredNode_");
const int ColoredNode::NodecolorType::Red(0);
const int ColoredNode::NodecolorType::Blue(1);
ColoredNode::NodecolorType::NodecolorType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int ColoredNode::Nodecolor::Red(0);
const int ColoredNode::Nodecolor::Blue(1);
const std::string ColoredNode::type__("ColoredNode");
const std::string ColoredNode::table__("ColoredNode_");
std::vector < std::pair< std::string, std::string > > ColoredNode::nodecolor_values;
const litesql::FieldType ColoredNode::Nodecolor("nodecolor_",A_field_type_integer,table__);
void ColoredNode::defaults() {
    nodecolor = 0;
}
ColoredNode::ColoredNode(const litesql::Database& db)
     : Node(db), nodecolor(Nodecolor) {
    defaults();
}
ColoredNode::ColoredNode(const litesql::Database& db, const litesql::Record& rec)
     : Node(db, rec), nodecolor(Nodecolor) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: nodecolor = convert<const std::string&, int>(rec[3]);
        nodecolor.setModified(false);
    }
}
ColoredNode::ColoredNode(const ColoredNode& obj)
     : Node(obj), nodecolor(obj.nodecolor) {
}
const ColoredNode& ColoredNode::operator=(const ColoredNode& obj) {
    if (this != &obj) {
        nodecolor = obj.nodecolor;
    }
    Node::operator=(obj);
    return *this;
}
std::string ColoredNode::insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs) {
    tables.push_back(table__);
    litesql::Record fields;
    litesql::Record values;
    fields.push_back("id_");
    values.push_back(id);
    fields.push_back(nodecolor.name());
    values.push_back(nodecolor);
    nodecolor.setModified(false);
    fieldRecs.push_back(fields);
    valueRecs.push_back(values);
    return Node::insert(tables, fieldRecs, valueRecs);
}
void ColoredNode::create() {
    litesql::Record tables;
    litesql::Records fieldRecs;
    litesql::Records valueRecs;
    type = type__;
    std::string newID = insert(tables, fieldRecs, valueRecs);
    if (id == 0)
        id = newID;
}
void ColoredNode::addUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, nodecolor);
    Node::addUpdates(updates);
}
void ColoredNode::addIDUpdates(Updates& updates) {
    prepareUpdate(updates, table__);
    updateField(updates, table__, id);
}
void ColoredNode::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    Node::getFieldTypes(ftypes);
    ftypes.push_back(Nodecolor);
}
void ColoredNode::delRecord() {
    deleteFromTable(table__, id);
    Node::delRecord();
}
void ColoredNode::delRelations() {
}
void ColoredNode::update() {
    if (!inDatabase) {
        create();
        return;
    }
    Updates updates;
    addUpdates(updates);
    if (id != oldKey) {
        if (!typeIsCorrect()) 
            upcastCopy()->addIDUpdates(updates);
        else
            addIDUpdates(updates);
    }
    litesql::Persistent::update(updates);
    oldKey = id;
}
void ColoredNode::del() {
    if (!typeIsCorrect()) {
        std::unique_ptr<ColoredNode> p(upcastCopy());
        p->delRelations();
        p->onDelete();
        p->delRecord();
    } else {
        delRelations();
        onDelete();
        delRecord();
    }
    inDatabase = false;
}
bool ColoredNode::typeIsCorrect() const {
    return type == type__;
}
std::unique_ptr<ColoredNode> ColoredNode::upcast() const {
    return unique_ptr<ColoredNode>(new ColoredNode(*this));
}
std::unique_ptr<ColoredNode> ColoredNode::upcastCopy() const {
    ColoredNode* np = new ColoredNode(*this);
    np->nodecolor = nodecolor;
    np->inDatabase = inDatabase;
    return unique_ptr<ColoredNode>(np);
}
std::ostream & operator<<(std::ostream& os, ColoredNode o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.nodecolor.name() << " = " << o.nodecolor << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
GraphDatabase::GraphDatabase( const std::string& backendType, const std::string& connInfo)
     : litesql::Database(backendType, connInfo) {
    initialize();
}
std::vector<litesql::Database::SchemaItem> GraphDatabase::getSchema() const {
    vector<Database::SchemaItem> res;
    string TEXT = backend->getSQLType(A_field_type_string);
    string rowIdType = backend->getRowIDType();
    res.push_back(Database::SchemaItem("schema_","table","CREATE TABLE schema_ (name_ "+TEXT+", type_ "+TEXT+", sql_ "+TEXT+")"));
    if (backend->supportsSequences()) {
        res.push_back(Database::SchemaItem("Node_seq","sequence",backend->getCreateSequenceSQL("Node_seq")));
        res.push_back(Database::SchemaItem("Tag_seq","sequence",backend->getCreateSequenceSQL("Tag_seq")));
        res.push_back(Database::SchemaItem("Property_seq","sequence",backend->getCreateSequenceSQL("Property_seq")));
    }
    res.push_back(Database::SchemaItem("Node_","table","CREATE TABLE Node_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"512") + "" +")"));
    res.push_back(Database::SchemaItem("Tag_","table","CREATE TABLE Tag_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"512") + "" +")"));
    res.push_back(Database::SchemaItem("Property_","table","CREATE TABLE Property_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"512") + "" +",value_ " + backend->getSQLType(A_field_type_string,"") + "" +")"));
    res.push_back(Database::SchemaItem("ColoredNode_","table","CREATE TABLE ColoredNode_ (id_ " + rowIdType + ",nodecolor_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Node_Property_NodeProperties","table","CREATE TABLE Node_Property_NodeProperties (Node1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Property2_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Node_Tag_NodeTags","table","CREATE TABLE Node_Tag_NodeTags (Node1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Tag2_ " + backend->getSQLType(A_field_type_integer,"") + "" +",type_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Node_Node_Edges","table","CREATE TABLE Node_Node_Edges (Node1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Node2_ " + backend->getSQLType(A_field_type_integer,"") + "" +",type_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Node_id_idx","index","CREATE INDEX Node_id_idx ON Node_ (id_)"));
    res.push_back(Database::SchemaItem("Tag_id_idx","index","CREATE INDEX Tag_id_idx ON Tag_ (id_)"));
    res.push_back(Database::SchemaItem("Property_id_idx","index","CREATE INDEX Property_id_idx ON Property_ (id_)"));
    res.push_back(Database::SchemaItem("O01c3af6048213c795ab854fc81f7a","index","CREATE INDEX O01c3af6048213c795ab854fc81f7a ON Node_Property_NodeProperties (Node1_)"));
    res.push_back(Database::SchemaItem("Odab4f9ee442a3b7752d9ff8b0b1d4","index","CREATE INDEX Odab4f9ee442a3b7752d9ff8b0b1d4 ON Node_Property_NodeProperties (Property2_)"));
    res.push_back(Database::SchemaItem("O94a4b4afd6e3c221b5dfb6546dc2d","index","CREATE INDEX O94a4b4afd6e3c221b5dfb6546dc2d ON Node_Property_NodeProperties (Node1_,Property2_)"));
    res.push_back(Database::SchemaItem("Node_Tag_NodeTagsNode1_idx","index","CREATE INDEX Node_Tag_NodeTagsNode1_idx ON Node_Tag_NodeTags (Node1_)"));
    res.push_back(Database::SchemaItem("Node_Tag_NodeTagsTag2_idx","index","CREATE INDEX Node_Tag_NodeTagsTag2_idx ON Node_Tag_NodeTags (Tag2_)"));
    res.push_back(Database::SchemaItem("Node_Tag_NodeTags_all_idx","index","CREATE INDEX Node_Tag_NodeTags_all_idx ON Node_Tag_NodeTags (Node1_,Tag2_)"));
    res.push_back(Database::SchemaItem("Node_Node_EdgesNode1_idx","index","CREATE INDEX Node_Node_EdgesNode1_idx ON Node_Node_Edges (Node1_)"));
    res.push_back(Database::SchemaItem("Node_Node_EdgesNode2_idx","index","CREATE INDEX Node_Node_EdgesNode2_idx ON Node_Node_Edges (Node2_)"));
    res.push_back(Database::SchemaItem("Node_Node_Edges_all_idx","index","CREATE INDEX Node_Node_Edges_all_idx ON Node_Node_Edges (Node1_,Node2_)"));
    return res;
}
void GraphDatabase::initialize() {
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;
    ColoredNode::initValues();
}
}
