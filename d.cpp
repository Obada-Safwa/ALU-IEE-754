vector <char> MainWindow::algoritmoMultiplicacion(Numero numeroA, Numero numeroB){

    vector<char> cero ={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    vector<char> inf ={'0','1','1','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

    //operandos cero
    if(numeroA.numero == 0 || numeroB.numero == 0){
        if((numeroA.numero == 0 && isinf(numeroB.numero)) || (numeroB.numero == 0 && isinf(numeroA.numero))){
            return inf;
        }
        else{
            return cero;
        }
    }

    //bits de mantisa
    int n = 23;

    //1.    SIGNO DEL PRODUCTO
    int signoResultado = (numeroA.signo + numeroB.signo) % 2;

    //2.    EXPONENTE DEL PRODUCTO
    int exponenteResultado = (numeroA.exponente + numeroB.exponente) - 127;

    //3.    MANTISA DEL PRODUCTO

    //3.1   P=mA*mB
    vector<char> mantisaA = numeroA.getMantisa();
    vector<char> mantisaB = numeroB.getMantisa();

    //Algoritmo producto enteros sin signo
    vector<char> P = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    int c1 = 0;

    for(int i = 0; i <= n; i++){
        if(mantisaA[n] == '1'){

            c1 = 0;
            for(int j = n; j >= 0; j--){
                int penI = P[j] - '0';
                int benI =  mantisaB[j] - '0';
                switch (benI + penI + c1) {
                case 3:
                    P[j] = '1';
                    c1 = 1;
                    break;
                case 2:
                    P[j] = '0';
                    c1 = 1;
                    break;
                case 1:
                    P[j] = '1';
                    c1 = 0;
                    break;
                default:
                    P[j] = '0';
                    c1 = 0;
                    break;
                }
            }


        }
        else{
            c1 = 0;
        }

        //Desplazar a la derecha P y mA

        char ultimoBitdeP = P[n];

        //se desplaza a la derecha mA introduciendo el ultimo bit de P
        mantisaA.insert(mantisaA.begin(), ultimoBitdeP);
        mantisaA.pop_back();

        //se desplaza a la derecha P introduciendo acarreo c1
        P.insert(P.begin(), c1 + '0');
        P.pop_back();

    }
    //Final Algoritmo producto enteros sin signo
    //Numero::printVector(P);

    //3.2   Si (Pn-1=0) entonces desplazar (P,A) un bit a la izquierda.
    if(P[0] == '0'){
        //se desplaza P a la izquierda
        P.push_back(mantisaA[0]);
        P.erase(P.begin());
        //se desplaza mA a la izquierda metiendo un 0
        mantisaA.push_back('0');
        mantisaA.erase(mantisaA.begin());
    }
    else{//Si no sumar 1 al exponente del producto
        exponenteResultado += 1;
    }

    //3.3   Bit de redondeo: r=an-1
    int r = mantisaA[0];

    //3.4   Bit sticky: st=OR(an-2,an-3,..a0)
    int st = 0;
    for(char c : mantisaA){
        if (c == '1') {
            st = 1;
        }
    }

    //3.5
    //Si (r=1 y st=1) O (r=1 y st=0 y P0=1) entonces P=P+1
    if((r == 1 && st == 1) || (r == 0 && st == 0 && P[n] == 1)){
        //P = P + 1
        vector<char> aux ={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'};
        int c2 = 0;

        for(int i = n; i >= 0; i--){
            int auxeni = aux[i] - '0';
            int peni = P[i] - '0';
            switch (auxeni + peni + c2) {
            case 3:
                P[i] = '1';
                c2 = 1;
                break;
            case 2:
                P[i] = '0';
                c2 = 1;
                break;
            case 1:
                P[i] = '1';
                c2 = 0;
                break;
            default:
                P[i] = '0';
                c2 = 0;
                break;
            }
        }
    }

    //Desbordamientos
    //Overflow
    if(exponenteResultado > 254){
        //01111111100000000000000000000000
        vector<char> inf ={'0','1','1','1','1','1','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
        return inf;
    }
    //Underflow
    if(exponenteResultado < 1){
        int t = 1 - exponenteResultado;
        if(t >= 24){
            //hay underflow
            vector<char> cero ={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
            return cero;
        }
        else{

            for(int i = 0; i < t; i++){
                //Desplazar P y A t bits a la derecha
                char ultimoBitdeP = P[n];

                //se desplaza a la derecha mA introduciendo el ultimo bit de P
                mantisaA.insert(mantisaA.begin(), ultimoBitdeP);
                mantisaA.pop_back();

                //se desplaza a la derecha P introduciendo 0
                P.insert(P.begin(), '0');
                P.pop_back();
            }

            exponenteResultado = 1;
        }
    }

    //denormales
    if(numeroA.getExponente() < 1 || numeroB.getExponente() < 1){
        if(exponenteResultado < 1){
            //hay underflow
            vector<char> cero ={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
            return cero;
        }
        else if(exponenteResultado < 1){
            int t1 = exponenteResultado - 1;
            int t2 = 0;
            bool encontrado = false;
            //t2 es un contador, se suma a este contador por cada 0 en la mantisa, hasta encontrar un 1
            for(int i = 0; i < (int)P.size(); i++){
                if(P[i] == '0'){
                    t2++;
                }else{
                    encontrado = true;
                    break;
                }
            }
            //si no se ha encontrado en P, se busca en mA
            if(encontrado == false){
                for(int i = 0; i < (int)mantisaA.size(); i++){
                    if(mantisaA[i] == '0'){
                        t2++;
                    }else{
                        break;
                    }
                }
            }

            //t = min t1 y t2
            int t = min(t1, t2);

            exponenteResultado -= t;

            //desplazar P,A t bits a la izquierda
            for(int i = 0; i < t; i++){
                //se desplaza P a la izquierda
                P.push_back(mantisaA[0]);
                P.erase(P.begin());
                //se desplaza mA a la izquierda metiendo un 0
                mantisaA.push_back('0');
                mantisaA.erase(mantisaA.begin());
            }




        }
        else if(exponenteResultado == 1){
            //resultado denormal
        }
    }


    //mp = P y final
    vector<char> resultado;

    resultado.push_back(signoResultado + '0');

    //si el exponente final es menor que la representacion, pasa a ser 1
    if(exponenteResultado < 1){
        exponenteResultado = 1;
    }
    vector<char> exponenteProductoBinario = Numero::rellenarExponente(Numero::enteroABinario(exponenteResultado));
    resultado.insert(resultado.end(), exponenteProductoBinario.begin(), exponenteProductoBinario.end());

    vector <char> mantisaResultado;
    mantisaResultado.assign(P.begin() + 1, P.end());

    resultado.insert(resultado.end(), mantisaResultado.begin(), mantisaResultado.end());

    return resultado;
}
