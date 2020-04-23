#ifndef graphdatabase_hpp
#define graphdatabase_hpp
#include "litesql.hpp"
namespace graph {
class Node;
class Tag;
class Property;
class ColoredNode;
class NodePropertyRelationNodeProperties {
public:
    class Row {
    public:
        litesql::Field<int> property;
        litesql::Field<int> node;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Node;
    static const litesql::FieldType Property;
    static void link(const litesql::Database& db, const graph::Node& o0, const graph::Property& o1);
    static void unlink(const litesql::Database& db, const graph::Node& o0, const graph::Property& o1);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<NodePropertyRelationNodeProperties::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class NodeTagRelationNodeTags {
public:
    class Row {
    public:
        litesql::Field<int> type;
        litesql::Field<int> tag;
        litesql::Field<int> node;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Node;
    static const litesql::FieldType Tag;
    static const litesql::FieldType Type;
    static void link(const litesql::Database& db, const graph::Node& o0, const graph::Tag& o1, int type=0);
    static void unlink(const litesql::Database& db, const graph::Node& o0, const graph::Tag& o1, int type);
    static void del(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    static litesql::DataSource<NodeTagRelationNodeTags::Row> getRows(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr());
    template <class T> static litesql::DataSource<T> get(const litesql::Database& db, const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
;
;
};
class NodeNodeRelationEdges {
public:
    class Row {
    public:
        litesql::Field<int> type;
        litesql::Field<int> node2;
        litesql::Field<int> node1;
        Row(const litesql::Database& db, const litesql::Record& rec=litesql::Record());
    };
    static const std::string table__;
    static const litesql::FieldType Node1;
    static const litesql::FieldType Node2;
    static const litesql::FieldType Type;
    static void link(const litesql::Database& db, const graph::Node& o0, const graph::Node& o1, int type=0);
    static void unlink(const litesql::Database& db, const graph::Node& o0, const graph::Node& o1, int type);
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
    class PropertiesHandle : public litesql::RelationHandle<Node> {
    public:
        PropertiesHandle(const Node& owner);
        void link(const Property& o0);
        void unlink(const Property& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Property> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<NodePropertyRelationNodeProperties::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class TagsHandle : public litesql::RelationHandle<Node> {
    public:
        TagsHandle(const Node& owner);
        void link(const Tag& o0, int type=0);
        void unlink(const Tag& o0, int type);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Tag> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<NodeTagRelationNodeTags::Row> getRows(const litesql::Expr& expr=litesql::Expr());
    };
    class EdgesHandle : public litesql::RelationHandle<Node> {
    public:
        EdgesHandle(const Node& owner);
        void link(const Node& o0, int type=0);
        void unlink(const Node& o0, int type);
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
    static void initValues();
protected:
    void defaults();
public:
    Node(const litesql::Database& db);
    Node(const litesql::Database& db, const litesql::Record& rec);
    Node(const Node& obj);
    const Node& operator=(const Node& obj);
    Node::PropertiesHandle properties();
    Node::TagsHandle tags();
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
    std::unique_ptr<Node> upcast() const;
    std::unique_ptr<Node> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, Node o);
class Tag : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class NodesHandle : public litesql::RelationHandle<Tag> {
    public:
        NodesHandle(const Tag& owner);
        void link(const Node& o0, int type=0);
        void unlink(const Node& o0, int type);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Node> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<NodeTagRelationNodeTags::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static void initValues();
protected:
    void defaults();
public:
    Tag(const litesql::Database& db);
    Tag(const litesql::Database& db, const litesql::Record& rec);
    Tag(const Tag& obj);
    const Tag& operator=(const Tag& obj);
    Tag::NodesHandle nodes();
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
    std::unique_ptr<Tag> upcast() const;
    std::unique_ptr<Tag> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, Tag o);
class Property : public litesql::Persistent {
public:
    class Own {
    public:
        static const litesql::FieldType Id;
    };
    class NodesHandle : public litesql::RelationHandle<Property> {
    public:
        NodesHandle(const Property& owner);
        void link(const Node& o0);
        void unlink(const Node& o0);
        void del(const litesql::Expr& expr=litesql::Expr());
        litesql::DataSource<Node> get(const litesql::Expr& expr=litesql::Expr(), const litesql::Expr& srcExpr=litesql::Expr());
        litesql::DataSource<NodePropertyRelationNodeProperties::Row> getRows(const litesql::Expr& expr=litesql::Expr());
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
    static const litesql::FieldType Value;
    litesql::Field<std::string> value;
    static void initValues();
protected:
    void defaults();
public:
    Property(const litesql::Database& db);
    Property(const litesql::Database& db, const litesql::Record& rec);
    Property(const Property& obj);
    const Property& operator=(const Property& obj);
    Property::NodesHandle nodes();
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
    std::unique_ptr<Property> upcast() const;
    std::unique_ptr<Property> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, Property o);
class ColoredNode : public Node {
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
    static const std::string type__;
    static const std::string table__;
protected:
    static std::vector < std::pair< std::string, std::string > > nodecolor_values;
public:
    static const litesql::FieldType Nodecolor;
    litesql::Field<int> nodecolor;
protected:
    void defaults();
public:
    ColoredNode(const litesql::Database& db);
    ColoredNode(const litesql::Database& db, const litesql::Record& rec);
    ColoredNode(const ColoredNode& obj);
    const ColoredNode& operator=(const ColoredNode& obj);
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
    std::unique_ptr<ColoredNode> upcast() const;
    std::unique_ptr<ColoredNode> upcastCopy() const;
};
std::ostream & operator<<(std::ostream& os, ColoredNode o);
class GraphDatabase : public litesql::Database {
public:
    GraphDatabase( const std::string& backendType, const std::string& connInfo);
protected:
    virtual std::vector<litesql::Database::SchemaItem> getSchema() const;
    static void initialize();
};
}
#endif
