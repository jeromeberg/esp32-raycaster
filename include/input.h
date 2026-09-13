#ifndef INPUT_H
#define INPUT_H

struct InputState {
    bool forward;
    bool rotateLeft;
    bool rotateRight;
};

class Input {
 public:
    void begin();
    InputState read() const;
};

#endif
