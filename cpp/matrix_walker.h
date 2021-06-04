class matrix_walker {
    public:
        virtual int size();
        virtual void teleport(int i, int j);
        virtual void move_left();
        virtual void move_right();
        virtual void move_up();
        virtual void move_down();
        virtual void set(int i, int j, int value);
        virtual void set_default();
};

class naive_matrix_walker : public matrix_walker{
    private:
        int n;
        int i, j;
        int *arr;
        int value;
        int translate(int i, int j);
    public:
        naive_matrix_walker(int n);
        int size();
        void teleport(int i, int j);
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        int get();
        void set(int i, int j, int value);
        void set_default();
};

class z_matrix_walker : public matrix_walker {
    private:
        int n;
        int n_pw2;
        int i, j;
        int odd_bits, even_bits;
        int *arr;
        int z_value;
        int value;
        int translate(int i, int j);

    public:
        z_matrix_walker(int n);
        int size();
        void teleport(int i, int j);
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        int get();
        void set(int i, int j, int value);
        void set_default();
};

class hilbert_matrix_walker : public matrix_walker{
    private:
        int n;
        int n_pw2;
        int i, j;
        int *arr;
        int h_value;
        int value;
        int translate(int i, int j);
        void move(int i, int j);

    public:
        hilbert_matrix_walker(int n);
        int size();
        void teleport(int i, int j);
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        int get();
        void set(int i, int j, int value);
        void set_default();
        void print();
};