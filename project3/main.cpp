#include <string>
#include <vector>
#include <iostream>
#include <fstream>


#include "../project4/Tuple.h"
#include "../project4/Scheme.h"
#include "../project4/Relation.h"
#include "../project4/Database.h"
#include "../project4/Interpreter.h"

#include "../project4/DatalogProgram.h"
#include "../project4/Parser.h"
#include "../project4/Scanner.h"

using namespace std;

Relation test_relation1() {
    vector<string> names = { "ID", "Name", "Major" };

    Scheme scheme(names);

    Relation relation("student", scheme);

    vector<string> values[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'32'", "'Bob'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
            {"'16'", "'Jim'", "'EE'"},
    };

    for (auto& value : values) {
        Tuple tuple(value);
//        cout << tuple.toString(scheme) << endl;
        relation.addTuple(tuple);
    }

    // print out relation
//    cout << "relation:" << endl;
//    cout << relation.toString();


    return relation;
}

Relation test_relation2() {
    vector<string> names = { "ID", "Name", "Major" };

    Scheme scheme(names);

    Relation relation("other student", scheme);

    vector<string> values[] = {
            {"'500'", "'Dane'", "'CS'"},
            {"'600'", "'Shaw'", "'EE'"},
            {"'550'", "'John'", "'EE'"},
            {"'400'", "'Jack'", "'CS'"},
    };

    for (auto& value : values) {
        Tuple tuple(value);
//        cout << tuple.toString(scheme) << endl;
        relation.addTuple(tuple);
    }

    // print out relation
//    cout << "relation:" << endl;
//    cout << relation.toString();


    return relation;
}

Relation test_select_relation_lab_3(Relation relation) {
    Relation result = relation.select(2, "'CS'");

    cout << "select Major='CS' result:" << endl;
    cout << result.toString();
    return result;
}



Database test_database_using_created_relations(Relation relation_1, Relation relation_2) {
    Database database;
    database.add_relation(relation_1);
    database.add_relation(relation_2);
    return database;
}

void print_test_database_1() {
    Relation relation_1 = test_relation1();
    cout << "relation_1:" << endl;
    cout << relation_1.toString() << endl;

    Relation relation_2 = test_relation2();
    cout << "relation_2:" << endl;
    cout << relation_2.toString() << endl;

    Database database_1 = test_database_using_created_relations(relation_1,relation_2);
    cout << "database_1:" << endl;
    database_1.toString();
}



DatalogProgram scan_and_parse(int argc, char* argv[]) {
    // declare variables
    string input_filename = argv[1];
    ifstream input_file;
    input_file.open(input_filename);
    stringstream ss;

    // processing file into string
    ss << input_file.rdbuf();
    string input_string = ss.str();
    input_file.close();

    // scanning string into a vector of tokens
    Scanner s = Scanner(input_string);
    s.scan_all_tokens();
    vector<Token> token_list = s.get_token_list();

    // parsing tokens into a datalog object
    Parser p = Parser(token_list);
    DatalogProgram test_datalog_program = p.parse();
    return test_datalog_program;
}





Database create_database_from_input_file(int argc, char* argv[]) {
    // scan input file --> tokens THEN parse tokens --> datalog program
    DatalogProgram input_datalog_program = scan_and_parse(argc, argv);
    cout << "===Datalog Program==="  << endl;
    cout << input_datalog_program.to_string_datalog() << endl;

    // initialize interpreter using datalog program
    Interpreter new_interpreter(input_datalog_program);

    // Interpret datalog program --> database
    cout << "===Database===" << endl;
    new_interpreter.interpret();
    Database new_database = new_interpreter.get_database();
//    cout << new_database.toString() << endl;
    return new_database;
}

void test_relational_operations(int argc, char* argv[]) {
    cout << "===Relational Operations===" << endl;
    Database new_database = create_database_from_input_file(argc, argv);
    Relation relation_1 = new_database.get_relation("student");

    // test select with index and value
    Relation relation_select_index_value = relation_1.select(0,1);
    cout << "relation_select_index_value" << endl;
    cout << relation_select_index_value.toString() << endl;

    // test select with two indexes
    Relation relation_select_index_index = relation_1.select(1,"'53'");
    cout << "relation_select_index_index" << endl;
    cout << relation_select_index_index.toString() << endl;

    // test project
    vector<int> projected_columns = {0,1};
    Relation relation_project = relation_1.project(projected_columns);
    cout << "relation_project" << endl;
    cout << relation_project.toString() << endl;


    // test rename
    vector<string> renamed_scheme_values = {"N","I","A","M"};
    Scheme renamed_scheme(renamed_scheme_values);
    Relation relation_rename = relation_1.rename(renamed_scheme);
    cout << "relation_rename" << endl;
    cout << relation_rename.toString() << endl;


}

void interpret_input_file(int argc, char* argv[]) {
    DatalogProgram input_datalog_program = scan_and_parse(argc, argv);
    Interpreter new_interpreter(input_datalog_program);
    new_interpreter.interpret();
}

int main(int argc, char* argv[]) {
//    test_relational_operations(argc, argv);
//    create_database_from_input_file(argc, argv);
    interpret_input_file(argc, argv);

}

// TODO
    // DONE: Modify Parameter class to contain a bool (isConstant)
        // Strings are constants, IDs are variables
    // Interpret queries in Interpreter.h
        // Implement evaluate_query(Predicate p)
        // Implement evaluate_all_queries()
        // Also implement the Query evaluation output string
    // DONE: Implement relational operations in Relation.h
        // select()
        // project()
        // rename()


// A relation object represents all components of a table
    // It stores the table's overall name
    // It stores the header row (scheme object)
    // It stores the data rows (set of tuple objects)
// A scheme object represents the row of attribute names  (the header row at the top row of the table)
// A tuple object represents the data in one row on the table (except for the header row)
    // A collection of tuples represents all the row data in a table
    // In a database, the tuple objects are the facts?
// Queries
    // Save for part B
// Rules
    // Save for project 4
// Rules
    //