class tensor_walker {
    public:
        virtual int size();
        virtual void teleport(int i, int j, int k);
        virtual void move_left();
        virtual void move_right();
        virtual void move_up();
        virtual void move_down();
        virtual void move_in();
        virtual void move_out();
        virtual void set(int i, int j, int k, int value);
        virtual void set_default();
};

class naive_tensor_walker : public tensor_walker {
    private:
        int n;
        int i, j, k;
        int *arr;
        int value;
        int translate(int i, int j, int k);
    public:
        naive_tensor_walker(int n);
        int size();
        void teleport(int i, int j, int k);
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        void move_in();
        void move_out();
        int get();
        void set(int i, int j, int k, int value);
        void set_default();
        void print();
};

class z_tensor_walker : public tensor_walker {
    private:
        int n;
        int n_pw2;
        int i, j, k;
        int i_bits, j_bits, k_bits;
        int *arr;
        int z_value;
        int value;
        int translate(int i, int j, int k);
    public:
        z_tensor_walker(int n);
        int size();
        void teleport(int i, int j, int k);
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        void move_in();
        void move_out();
        int get();
        void set(int i, int j, int k, int value);
        void set_default();
        void print();
};

class hilbert_tensor_walker : public tensor_walker {
    private:
        int n;
        int n_pw2;
        int i, j, k;
        int *arr;
        int h_value;
        int value;
        int translate(int i, int j, int k);
        void move(int i, int j, int k);
    public:
        hilbert_tensor_walker(int n);
        int size();
        void teleport(int i, int j, int k);
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        void move_in();
        void move_out();
        int get();
        void set(int i, int j, int k, int value);
        void set_default();
        void print();
};