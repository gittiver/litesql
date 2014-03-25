// include LiteSQL's header file and generated header file
#include <iostream>
#include "litesql.hpp"
#include "graphdatabase.hpp"


// no name collisions expected
using namespace litesql;
using namespace graph;

int main(int argc, char **argv) {

    try {
        // using SQLite3 as backend
        GraphDatabase db("sqlite3", "database=graph.db");
        // create tables, sequences and indexes
        db.verbose = true;
        
        if (db.needsUpgrade())
        {
          db.upgrade();
        }
        // start transaction
        db.begin();

        Node n1(db);
        n1.name = "n1";
        n1.nodecolor = Node::Nodecolor::Red;
        n1.update();
        
        Node n2(db);
        n2.name = "n1";
        n2.nodecolor = Node::Nodecolor::Red;
        n2.update();
        
        n1.edges().link(n2,0);
        n2.edges().link(n1,1);

        // count Persons
        cout << "There are " << select<Node>(db).count() 
             << " nodes." << endl;
	
        // select all Persons and order them by age
        vector<Node> nodes = select<Node>(db).orderBy(Node::Name).all();
        // show results
        for (vector<Node>::iterator i = nodes.begin(); i != nodes.end(); i++)
            cout << toString(*i) << endl;
             
        // select intersection of Jeff's and Jill's children and
        // iterate results with cursor
        Cursor<Node> cursor = intersect(n1.edges().get(), 
                                        n2.edges().get()).cursor();
        // select a non-existing Person
        try {
           select<Node>(db, Node::Name == "n3").one();
        } catch (NotFound e) {
            cout << "No Node with name n3" << endl;
        }
        // commit transaction
        db.commit();
        // clean up 
//        db.drop();
    } catch (Except e) {
        cerr << e << endl;
        return -1;
    }
    return 0;
}
