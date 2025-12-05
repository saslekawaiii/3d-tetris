#ifndef SHAPE_H_
#define SHAPE_H_

class Shape {
public:
    int enableCubes[3][3][3];  
    int size;                   
    int color;                  

    Shape();
    virtual ~Shape();

    void createShape();        
    void drawShape(float x, float y, float z);  
};

#endif /* SHAPE_H_ */
