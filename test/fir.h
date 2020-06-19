//
// Created by lucas on 07/02/2020.
//

#ifndef MAIN_FIR_H
#define MAIN_FIR_H

#include <data_flow.h>

template<class T>
DataFlow<T> *FIR(int id, int copies, T *coef, int taps, std::vector<T> *data_in, std::vector<T> *data_out) {
    auto df = new DataFlow<T>(id, "fir");
    int idx = 0;
    std::vector<Operator<T> *> in_cp;
    std::vector<Operator<T> *> out_cp;

    in_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        in_cp.push_back(new InputStream<T>(idx++, data_in[j]));
    }
    out_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        out_cp.push_back(new OutputStream<T>(idx++, data_out[j]));
    }
    for (int j = 0; j < copies; ++j) {
        Operator<T> *op, *op1, *op2;
        std::vector<Operator<T> *> add;
        add.reserve((unsigned long) taps - 1);
        for (int i = 0; i < taps; ++i) {
            auto m = new Multi<T>(idx++, coef[taps - i - 1]);
            if (i == 0) {
                op = new PassA<T>(idx++);
            } else {
                op = new Add<T>(idx++);
            }
            add.push_back(op);
            df->connect(in_cp[j], m, PORT_A);
            df->connect(m, op, PORT_A);
        }
        for (int i = 0; i < taps - 1; ++i) {
            op1 = add[i];
            op2 = add[i + 1];
            df->connect(op1, op2, PORT_B);
        }
        op1 = add[taps - 1];
        df->connect(op1, out_cp[j], PORT_A);
    }
    return df;
}

void run_fir() {

    std::vector<unsigned short> data_in[1] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    std::vector<unsigned short> data_out[1];
    unsigned short coef[4] = {1, 2, 3, 4};

    auto dataFlow = FIR(0, 1, coef, 4, data_in, data_out);
    dataFlow->compute();
    dataFlow->toJSON("../test/fir.json");
    dataFlow->toDOT("../test/fir.dot");

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

#endif //MAIN_FIR_H