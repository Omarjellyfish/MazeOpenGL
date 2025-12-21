#pragma once

enum CubeFace
{
    FACE_FRONT = 1 << 0,
    FACE_RIGHT = 1 << 1,
    FACE_BACK = 1 << 2,
    FACE_LEFT = 1 << 3,
    FACE_TOP = 1 << 4,
    FACE_BOTTOM = 1 << 5,

    FACE_ALL = FACE_FRONT | FACE_RIGHT | FACE_BACK |
    FACE_LEFT | FACE_TOP | FACE_BOTTOM
};
