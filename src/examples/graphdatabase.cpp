#include "graphdatabase.hpp"
namespace graph {
using namespace litesql;
const int NodeNodeRelationEdges::ColorType::Red(0);
const int NodeNodeRelationEdges::ColorType::Blue(1);
NodeNodeRelationEdges::ColorType::ColorType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
NodeNodeRelationEdges::Row::Row(const litesql::Database& db, const litesql::Record& rec)
         : color(NodeNodeRelationEdges::Color), node2(NodeNodeRelationEdges::Node2), node1(NodeNodeRelationEdges::Node1) {
    switch(rec.size()) {
    case 3:
        color = rec[2];
    case 2:
        node2 = rec[1];
    case 1:
        node1 = rec[0];
    }
}
const std::string NodeNodeRelationEdges::table__("Node_Node_Edges");
const litesql::FieldType NodeNodeRelationEdges::Node1("Node1_",A_field_type_integer,table__);
const litesql::FieldType NodeNodeRelationEdges::Node2("Node2_",A_field_type_integer,table__);
std::vector < std::pair< std::string, std::string > > NodeNodeRelationEdges::color_values;
const NodeNodeRelationEdges::ColorType NodeNodeRelationEdges::Color("color_",A_field_type_integer,table__,color_values);
void NodeNodeRelationEdges::initValues() {
    color_values.clear();
    color_values.push_back(make_pair("Red","0"));
    color_values.push_back(make_pair("Blue","1"));
}
void NodeNodeRelationEdges::link(const litesql::Database& db, const graph::Node& o0, const graph::Node& o1, int color) {
    Record values;
    Split fields;
    fields.push_back(Node1.name());
    values.push_back(o0.id);
    fields.push_back(Node2.name());
    values.push_back(o1.id);
    fields.push_back(Color.name());
    values.push_back(toString(color));
    db.insert(table__, values, fields);
    fields.clear();
    values.clear();
    fields.push_back(Node1.name());
    values.push_back(o1.id);
    fields.push_back(Node2.name());
    values.push_back(o0.id);
    fields.push_back(Color.name());
    values.push_back(toString(color));
    db.insert(table__, values, fields);
}
void NodeNodeRelationEdges::unlink(const litesql::Database& db, const graph::Node& o0, const graph::Node& o1, int color) {
    db.delete_(table__, (Node1 == o0.id && Node2 == o1.id && (Color == color)));
    db.delete_(table__, (Node1 == o1.id && Node2 == o0.id && (Color == color)));
}
void NodeNodeRelationEdges::del(const litesql::Database& db, const litesql::Expr& expr) {
    db.delete_(table__, expr);
}
litesql::DataSource<NodeNodeRelationEdges::Row> NodeNodeRelationEdges::getRows(const litesql::Database& db, const litesql::Expr& expr) {
    SelectQuery sel;
    sel.result(Node1.fullName());
    sel.result(Node2.fullName());
    sel.result(Color.fullName());
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
const int Node::NodecolorType::Red(0);
const int Node::NodecolorType::Blue(1);
Node::NodecolorType::NodecolorType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals)
         : litesql::FieldType(n,t,tbl,vals) {
}
const int Node::Nodecolor::Red(0);
const int Node::Nodecolor::Blue(1);
Node::EdgesHandle::EdgesHandle(const Node& owner)
         : litesql::RelationHandle<Node>(owner) {
}
void Node::EdgesHandle::link(const Node& o0, int color) {
    NodeNodeRelationEdges::link(owner->getDatabase(), *owner, o0, color);
}
void Node::EdgesHandle::unlink(const Node& o0, int color) {
    NodeNodeRelationEdges::unlink(owner->getDatabase(), *owner, o0, color);
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
std::vector < std::pair< std::string, std::string > > Node::nodecolor_values;
const litesql::FieldType Node::Nodecolor("nodecolor_",A_field_type_integer,table__);
void Node::initValues() {
    nodecolor_values.clear();
    nodecolor_values.push_back(make_pair<std::string, std::string>("Red","0"));
    nodecolor_values.push_back(make_pair<std::string, std::string>("Blue","1"));
}
void Node::defaults() {
    id = 0;
    nodecolor = 0;
}
Node::Node(const litesql::Database& db)
     : litesql::Persistent(db), id(Id), type(Type), name(Name), nodecolor(Nodecolor) {
    defaults();
}
Node::Node(const litesql::Database& db, const litesql::Record& rec)
     : litesql::Persistent(db, rec), id(Id), type(Type), name(Name), nodecolor(Nodecolor) {
    defaults();
    size_t size = (rec.size() > 4) ? 4 : rec.size();
    switch(size) {
    case 4: nodecolor = convert<const std::string&, int>(rec[3]);
        nodecolor.setModified(false);
    case 3: name = convert<const std::string&, std::string>(rec[2]);
        name.setModified(false);
    case 2: type = convert<const std::string&, std::string>(rec[1]);
        type.setModified(false);
    case 1: id = convert<const std::string&, int>(rec[0]);
        id.setModified(false);
    }
}
Node::Node(const Node& obj)
     : litesql::Persistent(obj), id(obj.id), type(obj.type), name(obj.name), nodecolor(obj.nodecolor) {
}
const Node& Node::operator=(const Node& obj) {
    if (this != &obj) {
        id = obj.id;
        type = obj.type;
        name = obj.name;
        nodecolor = obj.nodecolor;
    }
    litesql::Persistent::operator=(obj);
    return *this;
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
    fields.push_back(nodecolor.name());
    values.push_back(nodecolor);
    nodecolor.setModified(false);
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
    updateField(updates, table__, nodecolor);
}
void Node::addIDUpdates(Updates& updates) {
}
void Node::getFieldTypes(std::vector<litesql::FieldType>& ftypes) {
    ftypes.push_back(Id);
    ftypes.push_back(Type);
    ftypes.push_back(Name);
    ftypes.push_back(Nodecolor);
}
void Node::delRecord() {
    deleteFromTable(table__, id);
}
void Node::delRelations() {
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
        std::auto_ptr<Node> p(upcastCopy());
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
std::auto_ptr<Node> Node::upcast() const {
    return auto_ptr<Node>(new Node(*this));
}
std::auto_ptr<Node> Node::upcastCopy() const {
    Node* np = new Node(*this);
    np->id = id;
    np->type = type;
    np->name = name;
    np->nodecolor = nodecolor;
    np->inDatabase = inDatabase;
    return auto_ptr<Node>(np);
}
std::ostream & operator<<(std::ostream& os, Node o) {
    os << "-------------------------------------" << std::endl;
    os << o.id.name() << " = " << o.id << std::endl;
    os << o.type.name() << " = " << o.type << std::endl;
    os << o.name.name() << " = " << o.name << std::endl;
    os << o.nodecolor.name() << " = " << o.nodecolor << std::endl;
    os << "-------------------------------------" << std::endl;
    return os;
}
GraphDatabase::GraphDatabase(std::string backendType, std::string connInfo)
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
    }
    res.push_back(Database::SchemaItem("Node_","table","CREATE TABLE Node_ (id_ " + rowIdType + ",type_ " + backend->getSQLType(A_field_type_string,"") + "" +",name_ " + backend->getSQLType(A_field_type_string,"512") + "" +",nodecolor_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Node_Node_Edges","table","CREATE TABLE Node_Node_Edges (Node1_ " + backend->getSQLType(A_field_type_integer,"") + "" +",Node2_ " + backend->getSQLType(A_field_type_integer,"") + "" +",color_ " + backend->getSQLType(A_field_type_integer,"") + "" +")"));
    res.push_back(Database::SchemaItem("Node_id_idx","index","CREATE INDEX Node_id_idx ON Node_ (id_)"));
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
    Node::initValues();
    NodeNodeRelationEdges::initValues();
}
}
