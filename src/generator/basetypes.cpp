void initBaseTypes(Database& db) {
    Type* types[] = {new Type("integer", "int", "INTEGER"),
                    new Type("float", "float", "FLOAT"),
                    new Type("double", "double", "FLOAT"),
                    new Type("string", "string", "TEXT"),
                    new Type("boolean", "bool", "SMALLINT"),
                    new Type("date", "Date", "DATE"),
                    new Type("time", "Time", "VARCHAR(10)"),
                    new Type("datetime", "DateTime", "DATETIME")};

    Type* cppTypes[] = {new Type("string", "std::string", "TEXT"),
                    new Type("date", "litesql::Date", "DATE"),
                    new Type("time", "litesql::Time", "VARCHAR(10)"),
                    new Type("datetime", "litesql::DateTime", "DATETIME")};

    Type* pythonTypes[] = {new Type("double", "float", "FLOAT"),
                    new Type("string", "str", "TEXT"),
                    new Type("date", "litesql.Date", "DATE"),
                    new Type("time", "litesql.Time", "VARCHAR(10)"),
                    new Type("datetime", "litesql.DateTime", "DATETIME")};
                    
    IfTarget* cpp = new IfTarget("c++");
    IfTarget* python = new IfTarget("python");

    for (size_t i = 0; i < sizeof(types) / sizeof(types[0]); i++)
        db.types.push_back(types[i]);

    for (size_t i = 0; i < sizeof(cppTypes) / sizeof(cppTypes[0]); i++)
        cpp->types.push_back(cppTypes[i]);

    for (size_t i = 0; i < sizeof(pythonTypes) / sizeof(pythonTypes[0]); i++)
        python->types.push_back(pythonTypes[i]);

    db.ifTargets.push_back(cpp);
    db.ifTargets.push_back(python);
}
