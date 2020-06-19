//
// Created by lucas on 07/02/2020.
//

#ifndef MAIN_CHEBYSHEV_H
#define MAIN_CHEBYSHEV_H

#include <data_flow.h>

template<class T>
DataFlow<T> *chebyshev(int id, int copies, std::vector<T> *data_in, std::vector<T> *data_out) {
    auto df = new DataFlow<T>(id, "chebyshev");
    int idx = 0;
    std::vector<Operator<T> *> in;
    std::vector<Operator<T> *> out;
    for (int i = 0; i < copies; ++i) {
        in.push_back(new InputStream<T>(idx++, data_in[i]));
        out.push_back(new OutputStream<T>(idx++, data_out[i]));
    }
    for (int i = 0; i < copies; ++i) {

        auto reg1 = new PassA<T>(idx++);
        auto reg2 = new PassA<T>(idx++);
        auto reg3 = new PassA<T>(idx++);
        auto reg4 = new PassA<T>(idx++);
        auto reg5 = new PassA<T>(idx++);
        auto reg6 = new PassA<T>(idx++);
        auto reg7 = new PassA<T>(idx++);
        auto mult1 = new Multi<T>(idx++, 16);
        auto mult2 = new Mult<T>(idx++);
        auto sub1 = new Subi<T>(idx++, 20);
        auto mult3 = new Mult<T>(idx++);
        auto mult4 = new Mult<T>(idx++);
        auto add1 = new Addi<T>(idx++, 5);
        auto mult5 = new Mult<T>(idx++);

        df->connect(in[i], mult1, PORT_A);
        df->connect(in[i], reg1, PORT_A);
        df->connect(reg1, reg2, PORT_A);
        df->connect(reg2, reg5, PORT_A);
        df->connect(reg5, reg3, PORT_A);
        df->connect(reg3, reg6, PORT_A);
        df->connect(reg6, reg4, PORT_A);
        df->connect(reg1, mult2, PORT_A);
        df->connect(mult1, mult2, PORT_B);
        df->connect(mult2, sub1, PORT_A);
        df->connect(reg2, reg7, PORT_A);
        df->connect(reg7, mult3, PORT_A);
        df->connect(sub1, mult3, PORT_B);
        df->connect(reg3, mult4, PORT_A);
        df->connect(mult3, mult4, PORT_B);
        df->connect(mult4, add1, PORT_A);
        df->connect(reg4, mult5, PORT_A);
        df->connect(add1, mult5, PORT_B);
        df->connect(mult5, out[i], PORT_A);
    }

    return df;
}

void run_chebyshev() {

    std::vector<unsigned short> data_in[1] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    std::vector<unsigned short> data_out[1];

    unsigned short coef[4] = {1, 2, 3, 4};

    auto dataFlow = chebyshev(0, 1, data_in, data_out);
    dataFlow->compute();
    dataFlow->toJSON("../test/chebyshev.json");
    dataFlow->toDOT("../test/chebyshev.dot");


    for (const auto &dv:data_in) {
        for (auto v:dv) {
            cout << v << " ";
        }
        cout << endl;
    }
    cout << endl;
    for (const auto &dv:data_out) {
        for (auto v:dv) {
            cout << v << " ";
        }
        cout << endl;
    }
    cout << endl;
    delete dataFlow;
}

#endif //MAIN_CHEBYSHEV_H
