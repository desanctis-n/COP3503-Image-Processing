#include <iostream>
#include <fstream>
#include "Image.h"

using namespace std;

string CompareFiles(const string &filename, const string &examplefile) {
    ifstream file1(filename, ios_base::binary);
    ifstream file2(examplefile, ios_base::binary);
    char lhs;
    char rhs;
    int count = 0;
    while (!file1.eof()) {
        count ++;
        file1.read(reinterpret_cast<char *>(&lhs), 1);
        file2.read(reinterpret_cast<char *>(&rhs), 1);
        if (lhs != rhs)
            return "failed";
    }
    return "complete";
}
string CompareTask8() {
    if (CompareFiles("output/part8_b.tga", "examples/EXAMPLE_part8_b.tga") == "failed")
        return "failed";
    else if (CompareFiles("output/part8_g.tga", "examples/EXAMPLE_part8_g.tga") == "failed")
        return "failed";
    else if (CompareFiles("output/part8_r.tga", "examples/EXAMPLE_part8_r.tga") == "failed")
        return "failed";
    return "complete";
}

int main() {

    cout << endl;
    cout << "Executing tasks..." << endl;

//---------------------------- TASK 1 ----------------------------//

    Image test1_layer1;
    test1_layer1.ReadImage("input/layer1.tga");
    Image test1_pattern1;
    test1_pattern1.ReadImage("input/pattern1.tga");
    Image test1_product = test1_layer1.Multiply(test1_pattern1);
    test1_product.WriteImage("output/part1.tga");

    cout << "Task 1 " << CompareFiles("output/part1.tga", "examples/EXAMPLE_part1.tga") << endl;

//---------------------------- TASK 2 ----------------------------//

    Image test2_layer2;
    test2_layer2.ReadImage("input/layer2.tga");
    Image test2_car;
    test2_car.ReadImage("input/car.tga");
    Image test2_result = test2_car.Subtract(test2_layer2);
    test2_result.WriteImage("output/part2.tga");

    cout << "Task 2 " << CompareFiles("output/part2.tga", "examples/EXAMPLE_part2.tga") << endl;

//---------------------------- TASK 3 ----------------------------//

    Image test3_layer1;
    test3_layer1.ReadImage("input/layer1.tga");
    Image test3_pattern2;
    test3_pattern2.ReadImage("input/pattern2.tga");

    Image test3_temp = test3_layer1.Multiply(test3_pattern2);
    Image test3_text;
    test3_text.ReadImage("input/text.tga");

    Image test3_result = test3_text.Screen(test3_temp);
    test3_result.WriteImage("output/part3.tga");

    cout << "Task 3 " << CompareFiles("output/part3.tga", "examples/EXAMPLE_part3.tga") << endl;

//---------------------------- TASK 4 ----------------------------//

    Image test4_layer2;
    test4_layer2.ReadImage("input/layer2.tga");
    Image test4_circles;
    test4_circles.ReadImage("input/circles.tga");

    Image test4_temp = test4_layer2.Multiply(test4_circles);
    Image test4_pattern2;

    test4_pattern2.ReadImage("input/pattern2.tga");
    Image test4_result = test4_temp.Subtract(test4_pattern2);
    test4_result.WriteImage("output/part4.tga");

    cout << "Task 4 " << CompareFiles("output/part4.tga", "examples/EXAMPLE_part4.tga") << endl;

//---------------------------- TASK 5 ----------------------------//

    Image test5_layer1;
    test5_layer1.ReadImage("input/layer1.tga");
    Image test5_pattern1;
    test5_pattern1.ReadImage("input/pattern1.tga");
    Image test5_result = test5_pattern1.Overlay(test5_layer1);
    test5_result.WriteImage("output/part5.tga");

    cout << "Task 5 " << CompareFiles("output/part5.tga", "examples/EXAMPLE_part5.tga") << endl;

//---------------------------- TASK 6 ----------------------------//

    Image test6_car;
    test6_car.ReadImage("input/car.tga");
    Image test6_result = test6_car.AddColors(0, 200, 0);
    test6_result.WriteImage("output/part6.tga");

    cout << "Task 6 " << CompareFiles("output/part6.tga", "examples/EXAMPLE_part6.tga") << endl;

//---------------------------- TASK 7 ----------------------------//

    Image test7_car;
    test7_car.ReadImage("input/car.tga");
    Image test7_result = test7_car.ScaleColors(0, 1, 4);
    test7_result.WriteImage("output/part7.tga");

    cout << "Task 7 " << CompareFiles("output/part7.tga", "examples/EXAMPLE_part7.tga") << endl;

//---------------------------- TASK 8 ----------------------------//

    Image test8_car;
    test8_car.ReadImage("input/car.tga");
    test8_car.WriteToSplitChannels("output/part8_b.tga", "output/part8_g.tga", "output/part8_r.tga");

    cout << "Task 8 " << CompareTask8() << endl;

//---------------------------- TASK 9 ----------------------------//

    Image test9_image;
    test9_image.ReadFromChannels("input/layer_blue.tga", "input/layer_green.tga", "input/layer_red.tga");
    test9_image.WriteImage("output/part9.tga");

    cout << "Task 9 " << CompareFiles("output/part9.tga", "examples/EXAMPLE_part9.tga") << endl;

//---------------------------- TASK 10 ----------------------------//

    Image test10_text2;
    test10_text2.ReadImage("input/text2.tga");
    Image test10_result = test10_text2.Flip();
    test10_result.WriteImage("output/part10.tga");

    cout << "Task 10 " << CompareFiles("output/part10.tga", "examples/EXAMPLE_part10.tga") << endl;

   return 0;
}
