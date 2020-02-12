#ifndef OPERATOR_H
#define OPERATOR_H

#include <cmath>
#include <vector>
#include <defs.h>

template<class T>
class Operator {
private:
    int id;
    int dataFlowId;
    int opCode;
    int type;
    int level;
    T val;
    T _const;
    Operator<T> *srcA;
    Operator<T> *srcB;
    Operator<T> *branchIn;
    std::vector<Operator<T> *> dst;
    std::string name;
    bool end;
public:
    Operator<T>(int id, int op_code, int type, std::string name) : id(id), opCode(op_code), type(type), srcA(nullptr),
                                                                   srcB(nullptr), branchIn(nullptr), level(0),
                                                                   dataFlowId(-1), name(std::move(name)),end(false) {}

    Operator<T>(int id, int op_code, int type, std::string name, T constant) : id(id), opCode(op_code), type(type),
                                                                               srcA(nullptr), srcB(nullptr),
                                                                               branchIn(nullptr), _const(constant),
                                                                               level(0), dataFlowId(-1),
                                                                               name(std::move(name)),end(false) {}

    ~ Operator<T>() {
        srcA = nullptr;
        srcB = nullptr;
        branchIn = nullptr;
        dst.clear();
    }


    int getId() const {
        return id;
    }

    void setId(int i) {
        id = i;
    }

    void setEnd(bool e) {
        end = e;
    }

    bool isEnd() const {
        return end;
    }

    int getOpCode() const {
        return opCode;
    }

    void setOpCode(int op_code) {
        opCode = op_code;
    }

    int getType() const {
        return type;
    }

    void setType(int t) {
        type = t;
    }

    T getVal() const {
        return val;
    }

    void setVal(T v) {
        val = v;
    }

    T getConst() const {
        return _const;
    }

    void setConst(T v) {
        _const = v;
    }

    Operator<T> *getSrcA() const {
        return srcA;
    }

    void setSrcA(Operator<T> *src) {
        srcA = src;
    }

    Operator<T> *getSrcB() const {
        return srcB;
    }

    void setSrcB(Operator<T> *src) {
        srcB = src;
    }

    Operator<T> *getBranchIn() const {
        return branchIn;
    }

    void setBranchIn(Operator<T> *branch) {
        branchIn = branch;
    }

    std::vector<Operator<T> *> &getDst() {
        return dst;
    }

    virtual void compute() = 0;

    void setLevel(int l) {
        level = l;
    }

    int getLevel() {
        return level;
    }

    void setDataFlowId(int id) {
        dataFlowId = id;
    }

    int getDataFlowId() {
        return dataFlowId;
    }

    const std::string &getLabel() const {
        return name;
    }

};
typedef enum {
    OP_PASS_A = 0,
    OP_PASS_B = 1,
    OP_MIN = 2,
    OP_MAX = 3,
    OP_BEQ = 4,
    OP_BNE = 5,
    OP_SLT = 6,
    OP_SGT = 7,
    OP_ADD = 8,
    OP_SUB = 9,
    OP_MULT = 10,
    OP_XOR = 11,
    OP_AND = 12,
    OP_OR = 13,
    OP_NOT = 14,
    OP_SHL = 15,
    OP_SHR = 16,
    OP_MUX = 17,
    OP_ABS = 18
} op_opcode_t;

template<class T>
class Abs : public Operator<T> {
public:

    explicit Abs(int id) : Operator<T>(id, OP_ABS, OP_BASIC, "abs") {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = abs(Operator<T>::getSrcA()->getVal());
            Operator<T>::setVal(v);
        } else if (Operator<T>::getSrcB()) {
            auto v = abs(Operator<T>::getSrcB()->getVal());
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Add : public Operator<T> {
public:
    explicit Add(int id) : Operator<T>(id, OP_ADD, OP_BASIC, "add") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() + Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Addi : public Operator<T> {
public:
    Addi(int id, int constant) : Operator<T>(id, OP_ADD, OP_IMMEDIATE, "addi", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() + Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class And : public Operator<T> {
public:
    explicit And(int id) : Operator<T>(id, OP_AND, OP_BASIC, "and") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() & Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Andi : public Operator<T> {
public:
    Andi(int id, int constant) : Operator<T>(id, OP_AND, OP_IMMEDIATE, "andi", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() & Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Beq : public Operator<T> {
public:
    explicit Beq(int id) : Operator<T>(id, OP_BEQ, OP_BASIC, "beq") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() == Operator<T>::getSrcB()->getVal() ? 1 : 0;
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Beqi : public Operator<T> {
public:
    Beqi(int id, int constant) : Operator<T>(id, OP_BEQ, OP_IMMEDIATE, "beqi", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() == Operator<T>::getConst() ? 1 : 0;
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Bne : public Operator<T> {
public:
    explicit Bne(int id) : Operator<T>(id, OP_BNE, OP_BASIC, "bne") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() != Operator<T>::getSrcB()->getVal() ? 1 : 0;
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Bnei : public Operator<T> {
public:
    Bnei(int id, int constant) : Operator<T>(id, OP_BNE, OP_IMMEDIATE, "bnei", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() != Operator<T>::getConst() ? 1 : 0;
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class InputStream : public Operator<T> {
private:
    int index;
    std::vector<T> &data;

public:
    explicit InputStream(int id, std::vector<T> &data) : Operator<T>(id, OP_PASS_A, OP_IN, "input"), index(0),
                                                         data(data){}

    void compute() override {
        if (InputStream::index < data.size()) {
            auto v = InputStream::data[InputStream::index++];
            Operator<T>::setVal(v);
        } else {
            Operator<T>::setEnd(true);
        }
    }
};

template<class T>
class Max : public Operator<T> {
public:
    explicit Max(int id) : Operator<T>(id, OP_MAX, OP_BASIC, "max") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            if (Operator<T>::getSrcA()->getVal() > Operator<T>::getSrcB()->getVal()) {
                auto v = Operator<T>::getSrcA()->getVal();
                Operator<T>::setVal(v);
            } else {
                auto v = Operator<T>::getSrcB()->getVal();
                Operator<T>::setVal(v);
            }
        }
    }
};

template<class T>
class Maxi : public Operator<T> {
public:
    Maxi(int id, int constant) : Operator<T>(id, OP_MAX, OP_IMMEDIATE, "maxi", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            if (Operator<T>::getSrcA()->getVal() > Operator<T>::getConst()) {
                auto v = Operator<T>::getSrcA()->getVal();
                Operator<T>::setVal(v);
            } else {
                auto v = Operator<T>::getConst();
                Operator<T>::setVal(v);
            }
        }
    }
};

template<class T>
class Min : public Operator<T> {
public:
    explicit Min(int id) : Operator<T>(id, OP_MIN, OP_BASIC, "min") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            if (Operator<T>::getSrcA()->getVal() < Operator<T>::getSrcB()->getVal()) {
                auto v = Operator<T>::getSrcA()->getVal();
                Operator<T>::setVal(v);
            } else {
                auto v = Operator<T>::getSrcB()->getVal();
                Operator<T>::setVal(v);
            }
        }
    }
};

template<class T>
class Mini : public Operator<T> {
public:
    Mini(int id, int constant) : Operator<T>(id, OP_MIN, OP_IMMEDIATE, "mini", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            if (Operator<T>::getSrcA()->getVal() < Operator<T>::getConst()) {
                auto v = Operator<T>::getSrcA()->getVal();
                Operator<T>::setVal(v);
            } else {
                auto v = Operator<T>::getConst();
                Operator<T>::setVal(v);
            }
        }
    }
};

template<class T>
class Mult : public Operator<T> {
public:
    explicit Mult(int id) : Operator<T>(id, OP_MULT, OP_BASIC, "mult") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() * Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Multi : public Operator<T> {
public:
    Multi(int id, int constant) : Operator<T>(id, OP_MULT, OP_IMMEDIATE, "multi", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() * Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Mux : public Operator<T> {
public:
    explicit Mux(int id) : Operator<T>(id, OP_MUX, OP_BASIC, "mux") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB() && Operator<T>::getBranchIn()) {
            auto v = Operator<T>::getBranchIn()->getVal() ? Operator<T>::getSrcA()->getVal()
                                                          : Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Muxi : public Operator<T> {
public:
    Muxi(int id, int constant) : Operator<T>(id, OP_MUX, OP_IMMEDIATE, "muxi", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getBranchIn()) {
            auto v = Operator<T>::getBranchIn()->getVal() ? Operator<T>::getSrcA()->getVal() : Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Not : public Operator<T> {
public:
    explicit Not(int id) : Operator<T>(id, OP_NOT, OP_BASIC, "not") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = ~Operator<T>::getSrcA()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Or : public Operator<T> {
public:
    explicit Or(int id) : Operator<T>(id, OP_OR, OP_BASIC, "or") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() | Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Ori : public Operator<T> {
public:
    explicit Ori(int id, int constant) : Operator<T>(id, OP_OR, OP_IMMEDIATE, "ori", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() | Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class OutputStream : public Operator<T> {
private:
    int index;
    std::vector<T> &data;

public:
    explicit OutputStream(int id, std::vector<T> &data) : Operator<T>(id, OP_PASS_A, OP_OUT, "output"), index(0),
                                                          data(data) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal();
            Operator<T>::setVal(v);
            OutputStream::data.push_back(v);
        }

    }
};

template<class T>
class PassA : public Operator<T> {
public:
    explicit PassA(int id) : Operator<T>(id, OP_PASS_A, OP_BASIC, "reg") {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class PassB : public Operator<T> {
public:
    explicit PassB(int id) : Operator<T>(id, OP_PASS_B, OP_BASIC, "reg") {}

    void compute() override {
        if (Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class PassBi : public Operator<T> {
public:
    explicit PassBi(int id, int constant) : Operator<T>(id, OP_PASS_B, OP_IMMEDIATE, "reg", constant) {}

    void compute() override {
        auto v = Operator<T>::getConst();
        Operator<T>::setVal(v);
    }
};

template<class T>
class Sgt : public Operator<T> {
public:
    explicit Sgt(int id) : Operator<T>(id, OP_SGT, OP_BASIC, "sgt") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() > Operator<T>::getSrcB()->getVal() ? 1 : 0;
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Sgti : public Operator<T> {
public:
    explicit Sgti(int id, int constant) : Operator<T>(id, OP_SGT, OP_IMMEDIATE, "sgti", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() > Operator<T>::getConst() ? 1 : 0;
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Shl : public Operator<T> {
public:
    explicit Shl(int id) : Operator<T>(id, OP_SHL, OP_BASIC, "shl") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() << Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Shli : public Operator<T> {
public:
    explicit Shli(int id, int constant) : Operator<T>(id, OP_SHL, OP_IMMEDIATE, "shli", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() << Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Shr : public Operator<T> {
public:
    explicit Shr(int id) : Operator<T>(id, OP_SHR, OP_BASIC, "shr") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() >> Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Shri : public Operator<T> {
public:
    explicit Shri(int id, int constant) : Operator<T>(id, OP_SHR, OP_IMMEDIATE, "shri", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() >> Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Slt : public Operator<T> {
public:
    explicit Slt(int id) : Operator<T>(id, OP_SLT, OP_BASIC, "slt") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() < Operator<T>::getSrcB()->getVal() ? 1 : 0;
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Slti : public Operator<T> {
public:
    explicit Slti(int id, int constant) : Operator<T>(id, OP_SLT, OP_IMMEDIATE, "slti", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() < Operator<T>::getConst() ? 1 : 0;
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Sub : public Operator<T> {
public:
    explicit Sub(int id) : Operator<T>(id, OP_SUB, OP_BASIC, "sub") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() - Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Subi : public Operator<T> {
public:
    Subi(int id, int constant) : Operator<T>(id, OP_SUB, OP_IMMEDIATE, "subi", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() - Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Xor : public Operator<T> {
public:
    explicit Xor(int id) : Operator<T>(id, OP_XOR, OP_BASIC, "xor") {}

    void compute() override {
        if (Operator<T>::getSrcA() && Operator<T>::getSrcB()) {
            auto v = Operator<T>::getSrcA()->getVal() ^Operator<T>::getSrcB()->getVal();
            Operator<T>::setVal(v);
        }
    }
};

template<class T>
class Xori : public Operator<T> {
public:
    Xori(int id, int constant) : Operator<T>(id, OP_XOR, OP_IMMEDIATE, "xori", constant) {}

    void compute() override {
        if (Operator<T>::getSrcA()) {
            auto v = Operator<T>::getSrcA()->getVal() ^Operator<T>::getConst();
            Operator<T>::setVal(v);
        }
    }
};
#endif
