# include <iostream>
using namespace std;
# include <map>
# include <vector>
# include <iomanip>
# include <limits>

int main(){

    // Variables for GPA, CGPA, semester and previous results
    int num_courses;
    float CGPA;
    float GPA ;
    int semester;
    double prev_CGPA;
    double prev_credit_hours;


    // Read the number of courses (must be 1 or more)
    do{
        cout << "Enter the number of courses : ";
        cin >> num_courses;

        if(cin.fail()){
            cin.clear();
        cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        num_courses = 0;
        }

        } while (num_courses <= 0);
    vector<string> grade(num_courses);
    vector<int> credit_hour(num_courses);

    do{
        // Read the current semester number (must be 1 or more)
        cout << "Enter your current semester number: ";
        cin >> semester;

        if(cin.fail()){
            cin.clear();

        cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        semester = 0;
        }
        } while(semester < 1);

        // Map each letter grade to its grade points
    map<string, double> gradepoints = {
        {"A",4.0}, {"B+",3.5},{"B",3.0},
        {"C+",2.5},{"C",2.0},{"D+",1.5},
        {"D",1.0},{"E",0.5},{"F",0.0}

    };

    // Running totals: weighted grade points and credit hours
    double total_grade = 0.0 ;
    double total_credit_hour = 0.0;
    double total_credit_hours;

    cout << "Enter the grade & credit hour for each course" << endl;

    // Read each course and add up the weighted grade points and credit hours
    for(int i = 0; i < num_courses; i++){

        // Read a valid letter grade, then valid credit hours, for this course
        do{
        cout << "Enter alphabetical grade for course " << i+1 << " : ";
        cin >> grade[i];
        for(char &c : grade[i]){
        c = toupper(c);}
        }
        while(gradepoints.find(grade[i])== gradepoints.end());
        do{
        cout << "Enter the credit hours for course " << i+1 << " : ";
        cin >> credit_hour[i];

        if(cin.fail()){
        cin.clear();

        cin.ignore(numeric_limits <streamsize> ::max(), '\n');
        credit_hour[i] = 0;
        }
        }
        while(credit_hour[i] <= 0);

        // Add this course to the running totals
        total_grade += gradepoints[grade[i]] * credit_hour[i];
        total_credit_hour += credit_hour[i];

        // Update GPA (the last pass gives the final semester GPA)
        GPA = total_grade / total_credit_hour;
    }

      if(semester == 1){

           // First semester: CGPA equals GPA
           CGPA = GPA;

       }

       else{
            // Returning student: CGPA also uses the previous CGPA and credit hours
            do{
            cout << "Enter your previous CGPA : ";
            cin >> prev_CGPA;

            if(cin.fail()){
            cin.clear();

            cin.ignore(numeric_limits <streamsize> ::max(), '\n');
            prev_CGPA = -1;
            }
            } while(prev_CGPA < 0 || prev_CGPA > 4.0);

            do{
            cout << "Enter your previous credit hours : ";
            cin >> prev_credit_hours;

            if(cin.fail()){
            cin.clear();

            cin.ignore(numeric_limits <streamsize> ::max(), '\n');
            prev_credit_hours = -1;
            }
            } while(prev_credit_hours < 0);

            // Combine previous and current results to get the new CGPA
            total_grade = prev_CGPA * prev_credit_hours + total_grade;
            total_credit_hours = total_credit_hour + prev_credit_hours;
            CGPA = total_grade / total_credit_hours;
         }

    cout << "\n\n";

    cout << left << setw(12) << "COURSES" << setw(10) << "GRADE"
    <<setw(12)<< "CREDIT" <<right << setw(12)<< "GP POINT" << endl;
    cout << "-----------------------------------------------" << endl;

    // Display a table of each course, grade, credit hours and grade points
    for(int i = 0; i < num_courses; i++){
        string currentGrade = grade[i];
        double points = gradepoints[currentGrade];

        cout << left << setw(12) << i+1 << setw(10) <<currentGrade
             << setw(12) << credit_hour[i]
             << fixed << setprecision(1) <<right << setw(12)
             << points
             << endl;
    }

    cout << "\n\n";
    cout << fixed << setprecision(2);

    // Display the semester GPA and overall CGPA
    cout << "Your GPA for the semester is : " << GPA << endl;
    cout << "Your overall CGPA is : " << CGPA << endl;

    return 0;
}
