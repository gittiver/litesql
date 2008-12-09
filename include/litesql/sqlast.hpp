#ifndef _litesql_sqlast_hpp_
#define _litesql_sqlast_hpp_
#include "litesql/sql.hpp"
namespace litesql {
    namespace ast {

        class Raw;
        class Select;
        class Result;
        class Sources;
        class FromSource;
        class FromSelect;
        class CrossJoin;
        class InnerJoin;
        class LeftOuterJoin;
        class RightOuterJoin;
        class FullOuterJoin;
        class Union;
        class Intersect;
        class Except;
        class OrderBy;
        class Not;
        class UnaryMinus;
        class Eq;
        class Neq;
        class Lt;
        class LtEq;
        class Gt;
        class GtEq;
        class Or;
        class And;
        class Like;
        class RegExp;
        class IsNull;
        class In;
        class Concat;
        class Mod;
        class Div;
        class Mul;
        class Add;
        class Sub;
        class List;
        class ValueExpr;
        class FieldExpr;
        class FromSequence;
        class Insert;
        class ToTable;
        class Assign;
        class Update;
        class Delete;
        class CreateTable;
        class DropTable;
        class Field;
        class FieldConstraint;
        class FieldPrimaryKey;
        class FieldUnique;
        class FieldCheck;
        class FieldDefault;
        class FieldReference;
        class Cascade;
        class SetNull;
        class TablePrimaryKey;
        class CreateIndex;
        class DropIndex;

        /** AST translator base class */
        class Translator {
        public:
            virtual ~Translator() {}
            virtual sql::SQL* translate(Raw*)=0;
            virtual sql::SQL* translate(Select*)=0;
            virtual sql::SQL* translate(Result*)=0;
            virtual sql::SQL* translate(Sources*)=0;
            virtual sql::SQL* translate(FromSource*)=0;
            virtual sql::SQL* translate(FromSelect*)=0;
            virtual sql::SQL* translate(CrossJoin*)=0;
            virtual sql::SQL* translate(InnerJoin*)=0;
            virtual sql::SQL* translate(LeftOuterJoin*)=0;
            virtual sql::SQL* translate(RightOuterJoin*)=0;
            virtual sql::SQL* translate(FullOuterJoin*)=0;
            virtual sql::SQL* translate(Union*)=0;
            virtual sql::SQL* translate(Intersect*)=0;
            virtual sql::SQL* translate(Except*)=0;
            virtual sql::SQL* translate(OrderBy*)=0;
            virtual sql::SQL* translate(Not*)=0;
            virtual sql::SQL* translate(UnaryMinus*)=0;
            virtual sql::SQL* translate(Eq*)=0;
            virtual sql::SQL* translate(Neq*)=0;
            virtual sql::SQL* translate(Lt*)=0;
            virtual sql::SQL* translate(LtEq*)=0;
            virtual sql::SQL* translate(Gt*)=0;
            virtual sql::SQL* translate(GtEq*)=0;
            virtual sql::SQL* translate(Or*)=0;
            virtual sql::SQL* translate(And*)=0;
            virtual sql::SQL* translate(Like*)=0;
            virtual sql::SQL* translate(RegExp*)=0;
            virtual sql::SQL* translate(IsNull*)=0;
            virtual sql::SQL* translate(In*)=0;
            virtual sql::SQL* translate(Concat*)=0;
            virtual sql::SQL* translate(Mod*)=0;
            virtual sql::SQL* translate(Div*)=0;
            virtual sql::SQL* translate(Mul*)=0;
            virtual sql::SQL* translate(Add*)=0;
            virtual sql::SQL* translate(Sub*)=0;
            virtual sql::SQL* translate(List*)=0;
            virtual sql::SQL* translate(ValueExpr*)=0;
            virtual sql::SQL* translate(FieldExpr*)=0;
            virtual sql::SQL* translate(FromSequence*)=0;
            virtual sql::SQL* translate(Insert*)=0;
            virtual sql::SQL* translate(ToTable*)=0;
            virtual sql::SQL* translate(Assign*)=0;
            virtual sql::SQL* translate(Update*)=0;
            virtual sql::SQL* translate(Delete*)=0;
            virtual sql::SQL* translate(CreateTable*)=0;
            virtual sql::SQL* translate(DropTable*)=0;
            virtual sql::SQL* translate(Field*)=0;
            virtual sql::SQL* translate(FieldConstraint*)=0;
            virtual sql::SQL* translate(FieldPrimaryKey*)=0;
            virtual sql::SQL* translate(FieldUnique*)=0;
            virtual sql::SQL* translate(FieldCheck*)=0;
            virtual sql::SQL* translate(FieldDefault*)=0;
            virtual sql::SQL* translate(FieldReference*)=0;
            virtual sql::SQL* translate(Cascade*)=0;
            virtual sql::SQL* translate(SetNull*)=0;
            virtual sql::SQL* translate(TablePrimaryKey*)=0;
            virtual sql::SQL* translate(CreateIndex*)=0;
            virtual sql::SQL* translate(DropIndex*)=0;
        };


        /** Abstract syntax tree node */
        class AST {
        public:
            virtual ~AST() {}
            virtual std::string toString() const=0;
            virtual sql::SQL* translate(Translator& t) = 0;
        };

        /** AST base template which generates 
          a translate method */
        template <class Base, class T>
        class ASTBase : public Base {
        public:
            virtual sql::SQL* translate(Translator& translator) {
                return translator.translate(static_cast<T*>(this));
            }
        };

        /** Raw sql::SQL-injection */
        class Raw : public ASTBase<AST, Raw> { 
        public:
        };

        /** abstract statement */
        class Stmt : public AST {
        };

        /** sql::SQL SELECT statement */
        class Select : public ASTBase<Stmt, Select> {
        public:
        };

        /** SELECT result set field or expression */
        class Result : public ASTBase<AST, Result> {
        public:
        };

        /** FROM-expression in SELECT-clause */
        class Sources : public ASTBase<AST, Sources> {
        public:
        };

        /** Single FROM-source in SELECT-clause */
        class FromSource : public AST {
        };

        /** FROM (table)-expression in SELECT-clause */
        class FromTable : public ASTBase<FromSource, FromTable> {
        };

        /** FROM (select)-expression in SELECT-clause */
        class FromSelect : public ASTBase<FromSource, FromSelect> {
        };

        /** Base class for joins */
        class Join : public AST {
        };

        /** CROSS JOIN */
        class CrossJoin : public ASTBase<Join, CrossJoin> {
        };

        /** INNER JOIN _ ON _ */
        class InnerJoin : public ASTBase<Join, InnerJoin> {
        };

        /** LEFT OUTER JOIN _ ON _ */
        class LeftOuterJoin : public ASTBase<Join, LeftOuterJoin> {
        };

        /** RIGHT OUTER JOIN _ ON _ */
        class RightOuterJoin : public ASTBase<Join, RightOuterJoin> {
        };

        /** FULL OUTER JOIN _ ON _ */
        class FullOuterJoin : public ASTBase<Join, FullOuterJoin> {
        };

        /** Base class for compound operations */
        class CompoundOp : public AST {
        };

        /** UNION SELECT _ */
        class Union : public ASTBase<CompoundOp, Union> {
        };

        /** INTERSECT SELECT _ */
        class Intersect : public ASTBase<CompoundOp, Intersect> {
        };

        /** EXCEPT SELECT _ */
        class Except : public ASTBase<CompoundOp, Except> {
        };
        
        /** ORDER BY-part of SELECT-statement */
        class OrderBy : public ASTBase<AST, OrderBy> {

        };

        /** Base class for expressions */
        class Expr : public AST {
        };

        /** Base class for unary operator expressions */
        class UnOp : public Expr {
                    
        };

        /** NOT (_) */
        class Not : public ASTBase<UnOp, Not> {
        };

        /** -(_) */
        class UnaryMinus : public ASTBase<UnOp, UnaryMinus> {
        };

        /** Base class for binary operator expressions */
        class BinOp : public Expr {
        };

        /** _ = _ */
        class Eq : public ASTBase<BinOp, Eq> {
        };

        /** _ <> _ */
        class Neq : public ASTBase<BinOp, Neq> {
        };

        /** _ < _ */
        class Lt : public ASTBase<BinOp, Lt> {
        };

        /** _ <= _ */
        class LtEq : public ASTBase<BinOp, LtEq> {
        };

        /** _ > _ */
        class Gt : public ASTBase<BinOp, Gt> {
        };

        /** _ >= _ */
        class GtEq : public ASTBase<BinOp, GtEq> {
        };

        /** (_) OR (_) */
        class Or : public ASTBase<BinOp, Or> {
        };

        /** (_) AND (_) */
        class And : public ASTBase<BinOp, And> {
        };

        /** _ LIKE _ */
        class Like : public ASTBase<BinOp, Like> {
        };

        /** _ REGEXP _ */
        class RegExp : public ASTBase<BinOp, RegExp> {
        };

        /** _ IS NULL */
        class IsNull : public ASTBase<BinOp, IsNull> {
        };

        /** _ IN (_) */
        class In : public ASTBase<BinOp, In> {
        };

        /** _ || _ */
        class Concat : public ASTBase<BinOp, Concat> {
        };

        /** _ % _ */
        class Mod : public ASTBase<BinOp, Mod> {
        };

        /** _ / _ */
        class Div : public ASTBase<BinOp, Div> {
        };

        /** _ * _ */
        class Mul : public ASTBase<BinOp, Mul> {
        };

        /** _ + _ */
        class Add : public ASTBase<BinOp, Add> {
        };

        /** _ - _ */
        class Sub : public ASTBase<BinOp, Sub> {
        };

        /** Converts a list of values to a list expression */
        class List : public ASTBase<BinOp, List> {
        };

        /** Converts a value (int, string, whatever) to an expression */
        class ValueExpr : public ASTBase<BinOp, ValueExpr> {
        };

        /** Value of a field of the result set-expression */
        class FieldExpr : public ASTBase<BinOp, FieldExpr> {
        };


        /** The sequence used in the INSERT-operation */
        class FromSequence : public ASTBase<AST, FromSequence> {
        };

        /** Atomic multi-table insert that will assign rows with
            same new unique identifier to multiple tables */
        class Insert : public ASTBase<Stmt, Insert> {
        };

        /** Insertion to one table */
        class ToTable : public ASTBase<AST, ToTable> {
        };

        /** Assignment to a field */
        class Assign : public ASTBase<AST, Assign> {
        };

        /** UPDATE statement */
        class Update : public ASTBase<Stmt, Update> {
        };

        /** DELETE statement */
        class Delete : public ASTBase<Stmt, Delete> {
        };

        /** CREATE TABLE statement */
        class CreateTable : public ASTBase<Stmt, CreateTable> {
        };

        /** DROP TABLE statement */
        class DropTable : public ASTBase<Stmt, DropTable> {
        };

        /** Field definition to be used in CREATE table statement */
        class Field : public ASTBase<AST, Field> {
        };

        /** Base class for field constraints */
        class FieldConstraint : public ASTBase<AST, FieldConstraint> {
        };

        /** NOT NULL constraint for a field */
        class FieldNotNull : public ASTBase<FieldConstraint, FieldNotNull> {
        };

        /** PRIMARY KEY constraint for a field */
        class FieldPrimaryKey : public ASTBase<FieldConstraint, FieldPrimaryKey> {
        };

        /** UNIQUE constraint for a field */
        class FieldUnique : public ASTBase<FieldConstraint, FieldUnique> {
        };

        /** CHECK constraint for a field */
        class FieldCheck : public ASTBase<FieldConstraint, FieldCheck> {
        };

        /** DEFAULT value for a field */
        class FieldDefault : public ASTBase<FieldConstraint, FieldDefault> {
        };

        /** FOREIGN KEY reference for a field */
        class FieldReference : public ASTBase<FieldConstraint, FieldReference> {
        };

        /** Base class for actions */
        class Action : public AST {
        };

        /** CASCADE action (when a referenced row is deleted) */
        class Cascade : public ASTBase<Action, Cascade> {
        };

        /** SET NULL action (when a referenced row is deleted) */
        class SetNull : public ASTBase<Action, SetNull> {
        };

        /** Base class for table specific constraints */
        class TableConstraint : public AST {
        };

        /** multiple field PRIMARY KEY constraint */
        class TablePrimaryKey : public ASTBase<TableConstraint, 
                                               TablePrimaryKey> {
        };        

        /** CREATE INDEX statement */
        class CreateIndex : public ASTBase<Stmt, CreateIndex> {
        };

        /** DROP INDEX statement */
        class DropIndex : public ASTBase<Stmt, DropIndex> {
        };


    }
}

#endif

