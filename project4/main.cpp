#include "../project5/Interpreter.h"
#include "../project5/Scanner.h"
#include "../project5/Parser.h"

#include <fstream>
#include <iostream>


void test_joinable() {
    // test set 1: tests are_tuples_joinable logic by printing within loops
    cout << "---test set 1---" << endl;
    Scheme scheme1( { "A", "B" } );
    Scheme scheme2( { "B", "C" } );

    Tuple tuple1( {"'1'", "'2'"} );
    Tuple tuple2( {"'3'", "'4'"} );

    Relation::are_tuples_joinable(scheme1, scheme2, tuple1, tuple2);

    // test set 2: tests the return value of are_tuples_joinable
    cout << "---test set 2---" << endl;
    Scheme scheme3( { "X", "Y" } );
    Scheme scheme4( { "X", "Y", "Z" } );

    Tuple tuple3( {"'1'", "'4'"} );
    Tuple tuple4( {"'1'", "'2'", "'4'"} );

    cout << Relation::are_tuples_joinable(scheme1, scheme2, tuple1, tuple2) << endl;
    cout << Relation::are_tuples_joinable(scheme2, scheme3, tuple1, tuple2) << endl;
    cout << Relation::are_tuples_joinable(scheme3, scheme4, tuple1, tuple4) << endl;
    cout << Relation::are_tuples_joinable(scheme3, scheme4, tuple3, tuple4) << endl;
}

void test_join_duplicate_relations() {
    // set 1
    cout << "---set 1---" << endl;
    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));

    Relation set_1_joined_relation = studentRelation.join(studentRelation);
    cout << set_1_joined_relation.toString();
    cout << "end of set 1" << endl;
}

void test_join_partial_overlap_relations() {
    cout << "---set 2---" << endl;
    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));
    Relation courseRelation("courses", Scheme( {"ID", "Course"} ));

    vector<string> courseValues[] = {
            {"'42'", "'CS 100'"},
            {"'32'", "'CS 232'"},
    };

    for (auto& value : courseValues)
        courseRelation.addTuple(Tuple(value));


    Relation set_2_joined_relation = studentRelation.join(courseRelation);
    cout << set_2_joined_relation.toString() << endl;
    cout << "end of set 2" << endl;
}

void test_join_no_overlap_relations() {
    cout << "---set 4---" << endl;
    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));

    Relation petRelation("pets", Scheme( {"Species", "Color"} ));

    vector<string> petValues[] = {
            {"'Frog'", "'Green'"},
            {"'Cat'", "'Orange'"},
    };

    for (auto& value : petValues)
        petRelation.addTuple(Tuple(value));

    Relation set_3_joined_relation = studentRelation.join(petRelation);
    cout << set_3_joined_relation.toString();
    cout << "end of set 3" << endl;
}

void test_join_unjoinable_relations() {
    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));

    Relation wierdStudentRelation("wierd students", Scheme( {"ID", "Name", "Awkwardness"} ));

    vector<string> wierdValues[] = {
            {"'42'", "'Hudini'", "'Extremely Awkward'"},
            {"'64'", "'Albert'", "'Very Quiet'"},
    };
    for (auto& value : wierdValues)
        wierdStudentRelation.addTuple(Tuple(value));

    Relation set_4_joined_relation = studentRelation.join(wierdStudentRelation);
    cout << set_4_joined_relation.toString();
    cout << "end of set 4" << endl;
}

void test_union_duplicate_relations() {
    cout << "---union set 1---" << endl;
    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));

    Relation set_1_combined_relation = studentRelation.unionize(studentRelation);
    cout << set_1_combined_relation.toString();
    cout << "end of union set 1" << endl;
}

void test_union_relations_compatible() {
    cout << "---union set 2---" << endl;
    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));

    Relation otherStudentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> otherStudentValues[] = {
            {"'38'", "'Jude'", "'CS'"},
            {"'21'", "'Shaw'", "'EE'"},
    };

    for (auto& value : otherStudentValues)
        otherStudentRelation.addTuple(Tuple(value));

    Relation set_2_combined_relation = studentRelation.unionize(otherStudentRelation);
    cout << set_2_combined_relation.toString();
    cout << "end of union set 2" << endl;
}

void test_union_relations_uncompatible() {
    cout << "---union set 3---" << endl;
    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));

    Relation otherStudentRelation("students", Scheme( {"Identification", "Name", "Degree"} ));

    vector<string> otherStudentValues[] = {
            {"'38'", "'Jude'", "'CS'"},
            {"'21'", "'Shaw'", "'EE'"},
    };

    for (auto& value : otherStudentValues)
        otherStudentRelation.addTuple(Tuple(value));

    Relation set_3_combined_relation = studentRelation.unionize(otherStudentRelation);
    cout << set_3_combined_relation.toString();
    cout << "end of union set 3" << endl;
}

void test_project_change_column_order() {
    Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    vector<string> studentValues[] = {
            {"'42'", "'Ann'", "'CS'"},
            {"'64'", "'Ned'", "'EE'"},
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));

    vector<int> projected_columns = {2,1};
    Relation relation_project = studentRelation.project(projected_columns);
    cout << "relation_project" << endl;
    cout << relation_project.toString() << endl;
    cout << "end of relation_project" << endl;
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

void interpret_input_file(int argc, char* argv[]) {
    DatalogProgram input_datalog_program = scan_and_parse(argc, argv);
    Interpreter new_interpreter(input_datalog_program);
    new_interpreter.interpret();
//    cout << "end of interpreting" << endl;
}

int main(int argc, char* argv[]) {
//    test_join_duplicate_relations();
//    test_join_partial_overlap_relations();
//    test_join_no_overlap_relations();
//    test_join_unjoinable_relations();
//    test_union_duplicate_relations();
//    test_union_relations_compatible();
//    test_union_relations_uncompatible();
//    test_project_change_column_order();
    interpret_input_file(argc, argv);
}


// ---QUESTIONS-----
// In join()
    // why are the variables left and right marked as const?
    // What should the joined relation name be?
    // How should are_tuples_joinable() be implemented?

// are_tuples_joinable() thoughts
    // All Relations are are_tuples_joinable
        // Case 1: A joined Relation may have joined tuples with no matching scheme columns
            // All tuples from both relations are joined
            // QUESTION: If relation A has 5 tuples, and relation B has 3 tuples, will relation J contain 15 tuples?
        // A joined Relation may have joined tuples with matching columns
            // Each tuple is are_tuples_joinable if the matching columns have matching tuple values
            // If no tuples are are_tuples_joinable, the relation will be empty

    // All Schemes are are_tuples_joinable
        // Duplicate scheme values are added once (not twice) (Duplicates not required)
        // Unique scheme values are always added

    // Not all Tuples are are_tuples_joinable
        // If there are no matching scheme columns
            // all tuple values are joined
        // If there are matching scheme columns
            // the tuple is only are_tuples_joinable if the tuple values in the matching scheme columns also match
            // Otherwise, the tuple is NOT are_tuples_joinable

// ------TODO-------
// Relation.h
    // DONE finish implementing join()
        // DONE join_schemes()
        // DONE join_tuples()
        // DONE are_tuples_joinable()
        // DONE join()
    // DONE union()
    // DONE update project() to support evaluating rules
// Interpreter.h
    // TEST evaluate_rules()
        // TEST evaluate_one_rule()
    // UPDATE interpret() to evaluate the rules from the datalog program
    // UPDATE interpret() to print rule evaluation output
// Misc.
    // catch input errors? (the program must run to completion with normal exit status, despite input errors)
