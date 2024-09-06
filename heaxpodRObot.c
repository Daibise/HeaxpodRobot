// Importing necessary libraries for 3D plotting and animation

#define POINT_COUNT 100
#define TIP_CURVE_COUNTER 20

Hexapod hexapod = Hexapod(3, 2, 3);
float tip_points[3][TIP_CURVE_COUNTER]; // To store the tip points
int tip_curve_count = 0;

void drawBody() {
    // Draw body
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for head
    glPointSize(hexapod.body.head.f * 80);
    glBegin(GL_POINTS);
    glVertex3f(hexapod.body.head.x, hexapod.body.head.y, hexapod.body.head.z);
    glEnd();

    // Draw vertices
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < hexapod.body.vertex_count; i++) {
        glVertex3f(hexapod.body.vertices[i].x, hexapod.body.vertices[i].y, hexapod.body.vertices[i].z);
    }
    glEnd();
}

void drawLegs() {
    // Draw legs
    for (int k = 0; k < hexapod.leg_count; k++) {
        Leg leg = hexapod.legs[k];
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < 4; i++) {
            glVertex3f(leg.points_global[i].x, leg.points_global[i].y, leg.points_global[i].z);
        }
        glEnd();
    }
}

void update(int frame) {
    // Update IK
    float rot_x = -10 * sin((float)frame / POINT_COUNT * 2 * M_PI);
    float rot_y = -5 * cos((float)frame / POINT_COUNT * 2 * M_PI);
    hexapod.solve_ik(rot_x, rot_y, 0);

    // Update body and head
    drawBody();

    // Update legs
    drawLegs();

    // Update tip curve
    Point tip_point = hexapod.legs[5].get_ground_contact_point();
    if (tip_curve_count < TIP_CURVE_COUNTER) {
        tip_points[0][tip_curve_count] = tip_point.x;
        tip_points[1][tip_curve_count] = tip_point.y;
        tip_points[2][tip_curve_count] = tip_point.z;
        tip_curve_count++;
    } else {
        for (int i = 0; i < TIP_CURVE_COUNTER - 1; i++) {
            tip_points[0][i] = tip_points[0][i + 1];
            tip_points[1][i] = tip_points[1][i + 1];
            tip_points[2][i] = tip_points[2][i + 1];
        }
        tip_points[0][TIP_CURVE_COUNTER - 1] = tip_point.x;
        tip_points[1][TIP_CURVE_COUNTER - 1] = tip_point.y;
        tip_points[2][TIP_CURVE_COUNTER - 1] = tip_point.z;
    }

    // Draw tip curve here using OpenGL
}

int main(int argc, char** argv) {
    // Initialize GLUT and set up the display
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Hexapod Simulation");

    // Set up callbacks
    glutDisplayFunc(drawBody);
    glutIdleFunc(update);
    glutMainLoop();

    return 0;
}