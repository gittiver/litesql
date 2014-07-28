#ifndef graphdatabase_hpp
#define graphdatabase_hpp
#include "litesql.hpp"
namespace graph {
class Node;
class NodeNodeRelationEdges {
public:
    class ColorType : public litesql::FieldType {
    public:
        static const int Red;
        static const int Blue;
        ColorType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class Row {
    public:
        litesql::Field<int> color;
        litesql::Field<int> node2;
        litesql::Field<int> node1;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Node1;
    static const litesql::FieldType Node2;
protected:
    static std::vector < std::pair< std::string, std::string > > color_values;
public:
    static const NodeNodeRelationEdges::ColorType Color;
    static void initValues();
    static void link(const litesql::Database& db, const graph::Node& o0, const graph::Node& o1, int color=0);
    static void unlink(const litesql::Database& db, const graph::Node& o0, const graph::Node& o1, int color);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<NodeNodeRelationEdges::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<graph::Node> getNode1(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
    static litesql::DataSource<graph::Node> getNode2(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
};
class Node : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class NodecolorType : public litesql::FieldType {
    public:
        static const int Red;
        static const int Blue;
        NodecolorType(const std::string& n, AT_field_type t, const std::string& tbl, const litesql::FieldType::Values& vals=Values());
    };
    class Nodecolor {
    public:
        static const int Red;
        static const int Blue;
    };
    class EdgesHandle : public litesql::RelationHandle<Node> {
    public:
        EdgesHandle(const Node& owner);
        void link(const Node& o0, int color=0);
        void unlink(const Node& o0, int color);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Node> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<NodeNodeRelationEdges::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    static const std::string type__;
    static const std::string table__;
    static const std::string sequence__;
    static const litesql::FieldType Id;
    litesql::Field<int> id;
    static const litesql::FieldType Type;
    litesql::Field<std::string> type;
    static const litesql::FieldType Name;
    litesql::Field<std::string> name;
protected:
    static std::vector < std::pair< std::string, std::string > > nodecolor_values;
public:
    static const litesql::FieldType Nodecolor;
    litesql::Field<int> nodecolor;
    static void initValues();
protected:
    void defaults();
public:
    Node(const litesql::Database& db);
    Node(const litesql::Database& db, const litesql::Record& rec);
    Node(const Node& obj);
    const Node& operator=(const Node& obj);
    Node::EdgesHandle edges();
protected:
    std::string insert(litesql::Record& tables, litesql::Records& fieldRecs, litesql::Records& valueRecs);
    void create();
    virtual void addUpdates(Updates& updates);
    virtual void addIDUpdates(Updates& updates);
public:
    static void getFieldTypes(std::vector<litesql::FieldType>& ftypes);
protected:
    virtual void delRecord();
    virtual void delRelations();
public:
    virtual void update();
    virtual void del();
    virtual bool typeIsCorrect() const;
    std::auto_ptr<Node> upcast() const;
    std::auto_ptr<Node> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, Node o);
class GraphDatabase : public litesql::Database {
public:
    GraphDatabase(std::string backendType, std::string connInfo);
protected:
    virtual std::vector<litesql::Database::SchemaItem> getSchema() const;
    static void initialize();
};
}
#endif
