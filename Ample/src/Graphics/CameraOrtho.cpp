#include <GL/gl.h>
#include <GL/glu.h>

#include "CameraOrtho.h"

namespace ample::graphics
{
CameraOrtho::CameraOrtho(Vector2d<pixel_t> viewSize, Vector2d<pixel_t> viewPosition,
                         Vector2d<double> cameraSize, Vector3d<double> cameraPosition,
                         double ratio)
    : Camera(viewSize, viewPosition, cameraSize, cameraPosition, ratio)
{
    setPerspective(-cameraSize.x / 2.0, cameraSize.x / 2.0, -cameraSize.y / 2.0, cameraSize.y / 2.0, -100.0, 100.0);
}

CameraOrtho::CameraOrtho(Vector2d<pixel_t> viewSize, Vector2d<pixel_t> viewPosition,
                         Vector2d<double> cameraSize, Vector2d<double> cameraPosition,
                         double ratio)
    : CameraOrtho(viewSize, viewPosition, cameraSize, {cameraPosition.x, cameraPosition.y, 0.0}, ratio) {}

CameraOrtho::CameraOrtho(Vector2d<pixel_t> viewSize,
                         Vector2d<double> cameraSize,
                         double ratio)
    : CameraOrtho(viewSize, {0, 0}, cameraSize, {0, 0}, ratio) {}

CameraOrtho::CameraOrtho(Vector2d<pixel_t> cameraSize, double ratio)
    : CameraOrtho(cameraSize, {static_cast<double>(cameraSize.x), static_cast<double>(cameraSize.y)}, ratio) {}

void CameraOrtho::look()
{
    _viewport.set();
    glPushMatrix();
    glOrtho(_left, _right,
            _bottom, _top,
            _near, _far);
    glScaled(_scale.x, _scale.y, _scale.z);
    glRotated(_angle.x, 1.0, 0.0, 0.0);
    glRotated(_angle.y, 0.0, 1.0, 0.0);
    glRotated(_angle.z, 0.0, 0.0, 1.0);
    glTranslated(_position.x * _ratio,
                 _position.y * _ratio,
                 _position.z * _ratio);
}

void CameraOrtho::unlook()
{
    glPopMatrix();
}
} // namespace ample::graphics
