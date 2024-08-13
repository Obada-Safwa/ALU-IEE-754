#define SIMPLE_BIAS 127
#define SIMPLE_BITS 32
#define SIMPLE_EXPONENT 8
#define SIMPLE_MANTISSA 23

// DOUBLE PRECISSION CONSTANTS

#define DOUBLE_BIAS 1023
#define DOUBLE_BITS 64

// LIBRARIES

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std; // Workspace std shortcut

// Method signature
string int_to_binary(int int_value, int length);
string decimal_to_binary(string str);
int binary_to_int(string str);

/*
    Store the same value as for a real num
    and retrive each part.
*/
typedef union {
    float f_value;
    struct{
        unsigned int mantissa: 23;
        unsigned int exponent: 8;
        unsigned int sign: 1;
    } raw;
} simple_precision;


// Method signature



// Global variables
simple_precision s_datablock;



string int_to_binary(int int_value, int length){
    string str;
    int local_int = int_value;

    if(local_int == 0){
        str.insert(0, "0");
        return str;
    }

    while(local_int >= 1){
        // cout << " Local int: " << local_int << " local%2: " << local_int%2 << endl;
        if(local_int%2 == 0){
            str.insert(0, "0");
        } else {
            str.insert(0, "1");
        }
        local_int= local_int/2;
    }
    while(str.length() < length){
        str.insert(0, "0");
    }
    //cout << str << endl;
    return str;
}

string decimal_to_binary(float arg_input){
    string result;
    s_datablock.f_value = arg_input;


    result.append(int_to_binary(s_datablock.raw.sign, 1));
    result.append(int_to_binary(s_datablock.raw.exponent, 8));
    result.append(int_to_binary(s_datablock.raw.mantissa, 23));

    //  DEBUG:
    /*
    cout << " sign: " << s_datablock.raw.sign << " exponent: " << s_datablock.raw.exponent << " mantissa: " << s_datablock.raw.mantissa << endl;
    cout << " sign: " << int_to_binary(s_datablock.raw.sign, 1) << " exponent: " << int_to_binary(s_datablock.raw.exponent, 8) << " mantissa: " << int_to_binary(s_datablock.raw.mantissa, 23) << endl;

    */
    for(int i = result.length(); i<32 ; i++){
        result.append("0");
    }
    return result;
}

int binary_to_int(string str){
    int i = str.length() - 1, result = 0, increment;
    for(i;i>=0;i--){
        //cout << " Siendo i = " << i << " y Siendo l otro " << str.length()-i-1 << " el interior del string es "<< str[str.length()-i-1] << endl;
        if(str[str.length()-i-1] == '1'){
            increment = pow(2.0,i);
            //cout << " INCREMENTO "<< increment << endl;
            result += increment;
        }
    }
    //cout << "Result: " << result;
    return result;
}

string binary_to_decimal(string str){

    if (str.find("inf") != std::string::npos){
        return "inf";
    }
    string sign = str.substr(0,1);
    string exponent = str.substr(1,8);
    string mantisa = str.substr(9,32);
    //cout << sign << " " << exponent << " " << mantisa << endl;
    s_datablock.raw.sign = stoi(sign);
    s_datablock.raw.exponent = binary_to_int(exponent);
    s_datablock.raw.mantissa = binary_to_int(mantisa);

    std::ostringstream oss;
    oss << s_datablock.f_value ;
    //cout << s_datablock.f_value << endl;
    return oss.str();
}





// ----------------------------------------------------------------------------------
// ---------Algoritmo de Suma --------------------------------------------------
// ----------------------------------------------------------------------------------
/**
    In this method the operation addition it's realized.
    @param The first binary number in string format
    @param The second binary number in string format
    @return Result of the operation
*/
pair<string, int> addition_operation(string a_str, string b_str){

    int carry = 0, sum = 0, i = max(a_str.length(), b_str.length()) -1;
    string result;

    //Put zeros if the length is diferent
    for(int j = min(a_str.length(), b_str.length())-1 ; j < i ; j++){
        if(a_str.length() > b_str.length()){
            b_str.insert(0, "0");
        } else if (a_str.length() < b_str.length()) {
            a_str.insert(0, "0");
        }
    }

    for(i ; i>=0 ; i--){
        sum = (a_str[i] - '0') + (b_str[i] - '0') + carry;
        if(sum == 3){
            carry = 1;
            result.insert(0, "1");
        } else if (sum == 2){
            carry = 1;
            result.insert(0, "0");
        } else if (sum == 1){
            carry = 0;
            result.insert(0, "1");
        } else if (sum == 0){
            carry = 0;
            result.insert(0, "0");
        }
        sum = 0;
    }
    return make_pair(result, carry);
}

string complemento_2(string input_str){
    string result_str;
    for (char c: input_str){
        int j = c - '0';
        if(j == 0){
            result_str.append("1");
        } else {
            result_str.append("0");
        }
    }
    //cout << " Complemento 1 DEBUG: " << result_str << endl;

    pair<string, int> result_and_carry = addition_operation(result_str, "1");
    //cout << " Complemento 2 DEBUG: " << result_and_carry.first << endl;
    //cout << " Complemento 2 DEBUG: " << result_and_carry.second << endl;
    return result_and_carry.first;
}


string addition_algorithm(string bin_A, string bin_B, bool debug_mode){

    //Step 1: Initialization of variables.
    int store_bit = 0, round_bit = 0, sticky_bit = 0, n=24, shift=0;
    bool exchange = false, complement_p = false;
    string p;

    //Step 2: Separating the format (Mantissa has 24 bits at the end of the step because it's normalize)
    string sign_a = bin_A.substr(0,1);
    string exponent_a = bin_A.substr(1,8);
    string mantisa_a = bin_A.substr(9,32);

    string sign_b = bin_B.substr(0,1);
    string exponent_b = bin_B.substr(1,8);
    string mantisa_b = bin_B.substr(9,32);

    // Positive infinite: 0 11111111 00000000000000000000000
    // Negative infinite: 1 11111111 00000000000000000000000

    bool aInfe = exponent_a.find("11111111") != std::string::npos ;
    bool aInfm = mantisa_a.find("00000000000000000000000") != std::string::npos ;

    bool bInfe = exponent_b.find("11111111") != std::string::npos ;
    bool bInfm = mantisa_b.find("00000000000000000000000") != std::string::npos ;

    bool aZeroExp = exponent_a.find("00000000") != std::string::npos ;
    bool aZeroMan = mantisa_a.find("00000000000000000000000") != std::string::npos ;

    bool bZeroExp = exponent_b.find("00000000") != std::string::npos ;
    bool bZeroMan = mantisa_b.find("00000000000000000000000") != std::string::npos ;

    if(aZeroExp && aZeroMan && bZeroExp && bZeroMan){
        if(debug_mode){cout << " STEP 2 - TWO NUMBERS ARE ZERO " << mantisa_a << endl;}
        return "00000000000000000000000000000000";
    } else if(aZeroExp && aZeroMan){
        if(debug_mode){cout << " STEP 2 - A IS ZERO " << mantisa_a << endl;}
        return (sign_b + exponent_b + mantisa_b);
    } else if(bZeroExp && bZeroMan){
        if(debug_mode){cout << " STEP 2 - B IS ZERO " << mantisa_a << endl;}
        return (sign_a + exponent_a + mantisa_a);
    }

    if((aInfe && aInfm) || (bInfe && bInfm)) {
       return "inf";
    }

    mantisa_a.insert(0,"1");
    if(debug_mode){cout << " STEP 2 - Mantissa A normalize to size 24 " << mantisa_a << endl;}

    mantisa_b.insert(0,"1");
    if(debug_mode){ cout << " STEP 2 - Mantissa B normalize to size 24 " << mantisa_b << endl;}

    //Step 3: Calculate difference in the exponent

    int e_a = binary_to_int(exponent_a) - SIMPLE_BIAS;
    int e_b = binary_to_int(exponent_b) - SIMPLE_BIAS;
    if(debug_mode){cout << " STEP 3 - Exponent A: " << binary_to_int(exponent_a) << " Exponent B: " << binary_to_int(exponent_b) << endl;}
    if(e_a < e_b || (sign_a[0] - '0') > (sign_b[0] - '0') ){
        shift = e_b - e_a;
        if(debug_mode){
            cout << " STEP 3 - A before exchange: " << sign_a << " "  << exponent_a << " " << mantisa_a << endl;
            cout << " STEP 3 - B before exchange: " << sign_b << " "  << exponent_b << " " << mantisa_b << endl;
        }
        exchange = true;
        string index = sign_a;
        sign_a = sign_b;
        sign_b = index;

        index = exponent_a;
        exponent_a = exponent_b;
        exponent_b = index;

        index = mantisa_a;
        mantisa_a = mantisa_b;
        mantisa_b = index;
    } else {
        shift = e_a - e_b;
    }
    string exponent_sum = exponent_a;

    if(debug_mode){
        cout << " STEP 3 - A: " << sign_a << " "  << exponent_a << " " << mantisa_a << endl;
        cout << " STEP 3 - B: " << sign_b << " "  << exponent_b << " " << mantisa_b << endl;
    }


    //shift = e_a - e_b;
    if(debug_mode){
            cout << " STEP 2 - exponent_sum = " << exponent_sum << endl;
            cout << " STEP 3 - Shift = " << shift << endl;

    }

    //Step 4: Changing if there are of different sign
    if(binary_to_int(sign_a) != binary_to_int(sign_b)){
        if(debug_mode){
            cout << " STEP 4 - Different sign detected! " << endl;
            cout << " STEP 4 - Old mantissa_b: " << mantisa_b << endl;
        }

        mantisa_b = complemento_2(mantisa_b);
        if(debug_mode){cout << " STEP 4 - New mantissa_b: " << mantisa_b << endl;}
    } else {
        if(debug_mode){cout << " STEP 4 - Same signs, nothing to do in this step " << endl; }
    }

    //Step 5: Assign the working operator for the mantissa_b
    p = mantisa_b;
    if(debug_mode){cout << " STEP 5 - \"P\" operator for mantissa_b =  " << p << endl;}

    //Step 6: bits assignation
    if(shift>=1){
        store_bit = p[shift-1] - '0';
        if(debug_mode){cout << " STEP 6 - store_bit got the value: " << endl;}
    } else if(debug_mode){
        cout << " STEP 6 - store_bit doesn't exist " << endl;
    }

    if(shift>=2){
        round_bit = p[shift-2] - '0';
        if(debug_mode){cout << " STEP 6 - round_bit value is: " << round_bit << endl;}
    } else if(debug_mode){
        cout << " STEP 6 - round_bit doesn't exist " << endl;
    }

    for(int i = shift; i>0 && shift>=3 ; i--){
        if(p[i]-'0' == 1){
            sticky_bit = 1;
        }
    }
    //cout << " STEP 6 - sticky = " << sticky_bit << endl;
    //Step 7
    if(binary_to_int(sign_a) != binary_to_int(sign_b)){

        for (int i =0; i< 24 && i < shift; i++) {
            p = p.substr(0,n-1);
            p.insert(0, "1");
        }
        if(debug_mode){cout << " STEP 7 - Different Sign " << p <<endl;}

    } else {

        if (debug_mode){cout << " STEP 7 - Same sign " << endl;}
        for (int i =0; i< 24 && i < shift; i++) {
            p = p.substr(0,n-1);
            p.insert(0, "0");
        }
    }


    //Step 8
    if(debug_mode){
        cout << " STEP 8 - Mantisa = " << mantisa_a << endl;
        cout << " STEP 8 -       P = " << p << endl;
        cout << " STEP 8 - Mantisa_b = " << mantisa_b << endl;
    }
    pair<string, int> p_addition = addition_operation(mantisa_a, p);
    p = p_addition.first;
    int carry_1 = p_addition.second;
    if(debug_mode){
        cout << " STEP 8 -         + " << "______________________" << endl;
        cout << " STEP 8 -  Result = " << p << " C1 = " << carry_1 << endl;
    }

    //Step 9
    if((binary_to_int(sign_a) != binary_to_int(sign_b))&&(p[n-1]-'0'==1) && (carry_1==0)){
        p = complemento_2(p);
        complement_p = true;
        if(debug_mode){cout << " STEP 9 - p = " << p << endl;}

    } else if (debug_mode){
        cout << " STEP 9 - Condition not met " << endl;
    }

    //Step 10
    string p_substitute = p;
    if((binary_to_int(sign_a) == binary_to_int(sign_b))&&(carry_1==1)){
        if(store_bit == 1 || round_bit == 1 || sticky_bit ==1){
            sticky_bit =1;
            if(debug_mode){cout << " STEP 10 - sticky bit to 1" << endl;}

        }
        round_bit = p[0];
        p = p.substr(0,n-1);
        p.insert(0, string(1, '1'));
        pair<string, int> exponent_plus_one = addition_operation(exponent_sum, "1");
        exponent_sum = exponent_plus_one.first;

        if(debug_mode) {
            cout << " STEP 10 - exp_sum = " << exponent_sum << endl;
            cout << " STEP 10 - P = " << p << " adding in the left C1: " << carry_1 << endl;
        }

    } else {
        int k = 0;
        while((p_substitute[0]-'0') != 1 ){
            //p_substitute = p_substitute.substr(0,22);
            //p_substitute.insert(0, "0");
            p_substitute = p_substitute.substr(1,n-1);
            p_substitute.insert(n-2, "0");
            k++;
            //cout << " PASO 10 - P_sub = " << p_substitute << " has k = " << k << endl;
            if (k == 25) {
                break;
            }
        }
        //cout << " PASO 10 - EXP SUM 1 = " << exponent_sum << endl;
        if (k == 25) {
            int exp_minu_k = binary_to_int(exponent_sum)-binary_to_int(exponent_sum); //--k!=2 IMPORTANT--
            exponent_sum = int_to_binary(exp_minu_k, 8);
        } else {
            int exp_minu_k = binary_to_int(exponent_sum)-k; //--k!=2 IMPORTANT--
            exponent_sum = int_to_binary(exp_minu_k, 8);
        }
        //cout << " PASO 10 - EXP SUM 2 = " << exponent_sum << endl;
        if(k==0){
            round_bit = store_bit;
        }
        if (k > 1) {
            round_bit = 0;
            sticky_bit = 0;
            if (k <= 24) {
                p = p.substr(k, n-1);
                //cout << " PASO 10 - P after changing = " << p << endl;
                for (int i = 0; i < k; i++) {
                    p.append("0");
                }
                //p.insert(n-3, "0");
                //p.insert(n-3, "0");
                //cout << " PASO 10 - P after changing = " << p << endl;
            }
        }
    }

    //Step 11
    if((round_bit==1 && sticky_bit ==1)|| (round_bit==1 && sticky_bit==0 && p[23] ==1) ){
        pair<string, int> p_addition_one = addition_operation(p, "1");
        p = p_addition.first;
        int carry_2 = p_addition.second;
        if(carry_2 == 1){
            p = p.substr(0,n-1);
            p.insert(0, string(1, '1'));
            pair<string, int> exponent_plus_one_2 = addition_operation(exponent_sum, "1");
            exponent_sum = exponent_plus_one_2.first;
        }
    } else if(debug_mode){
        cout << " STEP 11 - Not going arround " << endl;
    }
    string mantisa_sum = p;
    string sign_sum;
    //Step 12
    if(exchange == false && complement_p == true){
        sign_sum = sign_b;
    } else {
        sign_sum = sign_a;
         //DEBUG
        if(debug_mode){cout << " STEP 12 - Sign of A taken as result " << endl;}
    }

    string sum;

    if(debug_mode){
        cout << " STEP 13 -    sign: " << sign_sum<< endl;
        cout << " STEP 13 -     exp: " << exponent_sum<< endl;
        cout << " STEP 13 - mantisa: " << mantisa_sum<< endl;
    }
    for (int i = exponent_sum.length(); i < 8 ; i++){
        exponent_sum.append("0");
    }
    mantisa_sum = mantisa_sum.substr(1,24);
    sum.append(sign_sum + exponent_sum + mantisa_sum);
    return sum;
}



string product_algorithm(string bin_A, string bin_B, bool debug_mode) {


    //Step 1: Initialization of variables.
    string sign_a = bin_A.substr(0,1);
    string exponent_a = bin_A.substr(1,8);
    string mantisa_a = bin_A.substr(9,32);

    string sign_b = bin_B.substr(0,1);
    string exponent_b = bin_B.substr(1,8);
    string mantisa_b = bin_B.substr(9,32);

    bool aInfe = exponent_a.find("11111111") != std::string::npos ;
    bool aInfm = mantisa_a.find("00000000000000000000000") != std::string::npos ;

    bool bInfe = exponent_b.find("11111111") != std::string::npos ;
    bool bInfm = mantisa_b.find("00000000000000000000000") != std::string::npos ;

    bool aZeroExp = exponent_a.find("00000000") != std::string::npos ;
    bool aZeroMan = mantisa_a.find("00000000000000000000000") != std::string::npos ;

    bool bZeroExp = exponent_b.find("00000000") != std::string::npos ;
    bool bZeroMan = mantisa_b.find("00000000000000000000000") != std::string::npos ;

    if(aZeroExp && aZeroMan && bZeroExp && bZeroMan){
        if(debug_mode){cout << " STEP 2 - TWO NUMBERS ARE ZERO " << mantisa_a << endl;}
        return "00000000000000000000000000000000";
    } else if(aZeroExp && aZeroMan){
        if(debug_mode){cout << " STEP 2 - A IS ZERO " << mantisa_a << endl;}
        return (sign_b + exponent_b + mantisa_b);
    } else if(bZeroExp && bZeroMan){
        if(debug_mode){cout << " STEP 2 - B IS ZERO " << mantisa_a << endl;}
        return (sign_a + exponent_a + mantisa_a);
    }

    if((aInfe && aInfm) || (bInfe && bInfm)) {
       return "inf";
    }

    // Calculating the sign
    string sign_pro;
    if((sign_a[0] - '0')==1 || (sign_b[0] - '0')==1){
        sign_pro = "1";
    } else {
        sign_pro = "0";
    }

    if(debug_mode){cout << " Sign is " << sign_pro << endl;}

    // Calculating the exponent
    string exponent_pro = int_to_binary( binary_to_int(exponent_a) + binary_to_int(exponent_b) - SIMPLE_BIAS, 8);
    if(debug_mode){cout << " Exponent is " << exponent_pro << endl;}



    mantisa_a.insert(0,"1");
    if(debug_mode){cout << " STEP 1 - Mantissa A normalize to size 24 " << mantisa_a << endl;}

    mantisa_b.insert(0,"1");
    if(debug_mode){ cout << " STEP 1 - Mantissa B normalize to size 24 " << mantisa_b << endl;}

    if (debug_mode) {
        cout << " STEP 1 - sign_a = " << sign_a << endl;
        cout << " STEP 1 - exponent_a = " << exponent_a << endl;
        cout << " STEP 1 - mantisa_a = " << mantisa_a << endl;

        cout << " STEP 1 - sign_b = " << sign_b << endl;
        cout << " STEP 1 - exponent_b = " << exponent_b << endl;
        cout << " STEP 1 - mantisa_b = " << mantisa_b << endl;
    }

    int store_bit = 0, round_bit = 0, sticky_bit = 0, n=24, shift=0, product_sign = binary_to_int(sign_a) * binary_to_int(sign_b);
    int product_exponent = binary_to_int(exponent_a) + binary_to_int(exponent_b);
    bool exchange = false, complement_p = false;
    //string p = int_to_binary(product_mantisa, 24);
    int len;
    string product_mantisa;

    string A = mantisa_a, B = mantisa_b;
    string P = "0000000000000000000000";
    int carry_1, counter;

    for (int i = 0 ; i<n; i++) {
        counter = 0;
        if (A[n-1] - '0' == 1) { // --IMPORTANT---
            pair<string, int> P_addition = addition_operation(B, P);
            P = P_addition.first;
            carry_1 = P_addition.second;

            if (debug_mode) { cout<<" STEP 1 - P = " << P << " CARRY = " << carry_1 << endl;}
        } else {
            /*
            P = P.substr(0, n-1);
            len = P.length();
            cout << " STEP 1 - P length = " << len << endl;
            P.insert(0, "0");
            len = P.length();
            cout << " STEP 1 - P length = " << len << endl;
            counter = 1;*/
            carry_1 = 0;
        }

        A = A.substr(0, n-1); //Delete far-right bit of A
        A.insert(0, 1, P[n-1]); // inserting the far-right bit of P in A

        if (counter == 0) {
            P = P.substr(0, n-1);
            len = P.length();
            cout << " STEP 1 - P length = " << len << endl;
            P.insert(0, string(int_to_binary(carry_1, 1)));
            len = P.length();
            cout << " STEP 1 - P length = " << len << endl;
        }

        cout << " STEP 1 - A = " << A << "  ITERATION = " << i << endl;
        cout << " STEP 1 - P = " << P << "  ITERATION = " << i << endl;
        product_mantisa = P + A;
        //A+= '0';
    }
    if (debug_mode) {
        len = product_mantisa.length();
        cout << " STEP 1 - product_mantisa length = " << len << endl;
    }
    //A = A.substr(0, n-1);
    //A+= '0';
    //string product_mantisa;
    //product_mantisa.append(int_to_binary(carry_1, 1) + P + A);
    unsigned int INTproduct_mantisa = binary_to_int(product_mantisa);

    if (debug_mode) {
        cout << " STEP 1 - product_sign = " << product_sign << endl;
        cout << " STEP 1 - product_exponent = " << product_exponent << endl;
        cout << " STEP 1 - A = " << A << endl;
        cout << " STEP 1 - product_mantisa = " << product_mantisa << endl;
        cout << " STEP 1 - INTproduct_mantisa = " << INTproduct_mantisa << endl;
        //cout << " STEP 1 - p = " << p << endl;
    }
    //p = "01000000000000000000000";

    // NOT STEP 2: Checking p and Shifting
    /*if (p[n-1] -'0' == 0) {
        p = p.substr(1, n-1);
        p+='0';
        if (debug_mode) {
            //cout << " STEP 2 - p is being shifted " << endl;
            cout << " STEP 2 - p is being shifted - p = " <<  p <<endl;
        }

    }*/


    //STEP 2: Shifting Product mantisa
    if (P[0]-'0'==0) { // ---IMPORTANT---

        product_mantisa = product_mantisa.substr(1, (n*2)-1);
        product_mantisa.push_back('0');
        if (debug_mode) {
            cout << " STEP 2 - product_mantisa = " << product_mantisa << endl;
        }
    }

    else {
            /*
        pair<string, int> product_exponent_addition = addition_operation(int_to_binary(product_exponent, 8), "1");
        product_exponent = binary_to_int(product_exponent_addition.first);
        //product_exponent++; //---DO I ADD IT LIKE THIS OR DO I ADD IT AS A BINARY---

        if (debug_mode) {
            cout << " STEP 2 - product_exponent = " << product_exponent << endl;
        }*/

        pair<string, int> exponent_plus_one = addition_operation(exponent_pro, "1");
        exponent_pro = exponent_plus_one.first;
    }

    //STEP 3: assign round_bit
    round_bit = A[0] - '0';
    if (debug_mode) {
        //cout << " STEP 3 - A[n-1] = " << A[n-1] << endl;
        //cout << " STEP 3 - n = " << n << endl;
        cout << " STEP 3 - round_bit = " << round_bit << endl;
    }

    //STEP 4: assign sticky_bit
    for (int i = 0; i<n; i++) {
        if (A[i]-'0'==1) {
            sticky_bit = 1;
        }
    }

    if (debug_mode) {
        cout << " STEP 4 - sticky_bit = " << sticky_bit << endl;
    }

    //STEP 5: Checking and If statement
    if ((round_bit==1 && sticky_bit == 1) || (round_bit==0 && sticky_bit == 0 && P[0]-'0'==1)) {
        pair<string, int> P_addition_one = addition_operation(P, "1");
        P = P_addition_one.first;
        int carry_2 = P_addition_one.second;
        if (debug_mode) {
            cout << " STEP 5 - P = " << P << endl;
            cout << " STEP 5 - carry = " << carry_2 << endl;
        }
    }

    //STEP 6: Overflows
    int min_exponent = min(binary_to_int(exponent_a), binary_to_int(exponent_a)) ;

    int t;
    if (debug_mode) {
        cout << " STEP 6 - min_exponent = " << min_exponent << endl;
    }

    if (product_exponent < min_exponent) {
        t = min_exponent-product_exponent;
        if (t>=n) {
            if (debug_mode) {
                cout << "STEP 6 - THERE IS AN UNDERFLOW" <<endl;
            }
        } else {
            for (int i = 0; i<t ; i++) {
                product_mantisa.substr(0, (n*2)- 1);
                product_mantisa.insert(0, "0");
                product_exponent = min_exponent;
                if (debug_mode) {
                    cout << " STEP 6 - NO UNDERFLOW " << endl;
                    cout << " STEP 6 - product_mantisa =  " << product_mantisa << endl;
                }
            }
        }
    }


    //STEP 7: operandos denormales
    string sub_product_mantisa = product_mantisa;
    if (product_exponent>min_exponent) {
        int t1 = product_exponent-min_exponent;
        int t2=0;
        while((sub_product_mantisa[0]-'0') != 1 ){
            sub_product_mantisa = sub_product_mantisa.substr(1,(n*2)-1);
            sub_product_mantisa.insert((n*2)-2, "0");
            t2++;
            if (debug_mode) {
                cout << " STEP 7 - sub_product_mantisa = " << sub_product_mantisa << endl;
            }
            if (t2 == 49) {
                break;
            }
        }
        t = min(t1, t2);
        if (debug_mode) {
            cout << " STEP 7 - t1 = " << t1 << endl;
            cout << " STEP 7 - t2 = " << t2 << endl;
            cout << " STEP 7 - t = " << t << endl;
        }
        product_exponent = product_exponent - t;
        for (int i =0; i<t; i++) {
            product_mantisa = product_mantisa.substr(1,(n*2)-1);
            product_mantisa.insert((n*2)-2, "0");
        }
        if (debug_mode) {
            cout << " STEP 7 - product_mantisa = " << product_mantisa << endl;
        }

    }

    if (product_exponent == min_exponent) {
        cout << "STEP 7 - The result is directly abnormal " << endl;
    }

    string sum;
    product_mantisa = product_mantisa.substr(1,23);
    sum.append(sign_pro + exponent_pro + product_mantisa);
    return sum;
}


int main() {
    //cout << addition_algorithm(decimal_to_binary(5), decimal_to_binary(5.4), true) << endl;
     /*cout << binary_to_decimal(addition_algorithm(decimal_to_binary(5), decimal_to_binary(5.4), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(5), decimal_to_binary(-5.4), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(-5), decimal_to_binary(5), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(1E38), decimal_to_binary(1E38), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(1E-37), decimal_to_binary(1E-37), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(1E38), decimal_to_binary(1E-37), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(1E38), decimal_to_binary(-1E-37), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(-1E38), decimal_to_binary(1E-37), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(1E40), decimal_to_binary(1E40), false)) << endl;
     cout << binary_to_decimal(addition_algorithm(decimal_to_binary(1E-40), decimal_to_binary(1E-40), false)) << endl;*/

     //cout << "The result of the addition is: " << binary_to_decimal(result) << endl;
    cout << " 5: " << decimal_to_binary(5)<< endl;;
    cout << " 2: " << decimal_to_binary(2)<< endl;;
     cout << product_algorithm(decimal_to_binary(1E38), decimal_to_binary(1E-37 ), true) << endl;
     cout << decimal_to_binary(10)<< endl;
}
