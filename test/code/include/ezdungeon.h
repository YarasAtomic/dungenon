extern "C" {

    #ifndef EZDUNGEON
    #define EZDUNGEON
    const unsigned int DUN_FULL_BLOCK = 1;
    const unsigned int DUN_PXY_WALL = 2;
    const unsigned int DUN_PXZ_WALL = 4;
    const unsigned int DUN_PYZ_WALL = 8;
    const unsigned int DUN_FULL_VAR = 16;
    const unsigned int DUN_PXY_VAR = 32;
    const unsigned int DUN_PXZ_VAR = 64;
    const unsigned int DUN_PYZ_VAR = 128;

    struct dungeonMatrix {
        unsigned int *** data;
        unsigned int size_x = 0;
        unsigned int size_y = 0;
        unsigned int size_z = 0;
        unsigned int err;
    };
    #endif
}