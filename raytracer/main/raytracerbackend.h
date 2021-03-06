#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QObject>
#include <QImage>
#include <QFuture>
#include <QFutureWatcher>
#include <QtCore/QFutureSynchronizer>
#include <QtConcurrent>
#include "canvas.h"
#include "camera.h"
#include "world.h"
#include "shapeqmlbridge.h"
#include "engine.h"

class RaytracerBackend : public QObject
{
    Q_OBJECT

public:
    explicit RaytracerBackend(QObject *parent = nullptr);

    ShapeQmlBridge* getSelectedObject() {
        return &m_selectedObject;
    }

    Q_PROPERTY(int progress MEMBER m_progress NOTIFY progressChanged)
    Q_PROPERTY(bool rendering MEMBER m_rendering NOTIFY renderingChanged)
    Q_PROPERTY(float fromX MEMBER m_fromX NOTIFY fromXChanged)
    Q_PROPERTY(float fromY MEMBER m_fromY NOTIFY fromYChanged)
    Q_PROPERTY(float fromZ MEMBER m_fromZ NOTIFY fromZChanged)
    Q_PROPERTY(float lastRenderTime MEMBER m_lastRenderTime NOTIFY imageReady)
    Q_PROPERTY(ShapeQmlBridge* selectedObject READ getSelectedObject NOTIFY selectedObjectChanged)

public slots:

    void render();
    void wireframe() const;
    void materialPreview();
    void switchChanged();
    void setViewportSize(int width, int height);

    void selectObject(float x, float y) {
        const Ray ray = ray_for_pixel(m_camera, x, y);
        const auto is = Raytracer::Engine::intersect_world(m_world, ray);
        const auto h = Raytracer::Engine::hit(is);
        if (h.has_value()) {
            auto shape_ptr = m_world.getShapePtrFromId(h.value().object->id);
            m_selectedObject.setShapePointer(shape_ptr);
            wireframe();
            materialPreview();

            emit selectedObjectChanged();
        }
    }

    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);

// end slots

signals:
    void imageReady(const QImage& image);
    void wireframesReady(const QImage& top, const QImage& left, const QImage& right, const QImage& perspective) const;
    void materialPreviewReady(const QImage& image);
    void renderingChanged();
    void progressChanged();
    void fromXChanged();
    void fromYChanged();
    void fromZChanged();
    void selectedObjectChanged();

private slots:
    void renderFinished();
    void materialPreviewFinished();
    void progressValueChanged(int value);

private:
    void drawLine(QImage& framebuffer, const QPointF& p1, const QPointF& p2, uint color = qRgb(255, 255, 255)) const;
    void drawLine(QPainter* p, const QPointF& p1, const QPointF& p2, uint color = qRgb(255, 255, 255)) const;

    QPointF convertWorldToScreenPoint(const Camera& camera, const Point& point) const;
    void setPixel(QImage& framebuffer, int x, int y, uint color = qRgb(255, 255, 255));
    void copyFrameBuffer(Canvas& from, QImage& to);
    void wireframe(QImage& framebuffer, const Camera& camera) const;
    void appendTransform(const Matrix<4,4>& transform, bool prepend = false);

    // Camera
    float m_fromX = 0;
    float m_fromY = 0;
    float m_fromZ = -8;

    float m_toX = 0;
    float m_toY = 0;
    float m_toZ = 1;

    // For rendering
    Canvas m_canvas;
    Camera m_camera;
    QImage m_framebuffer;

    World m_world;
    World m_previewWorld;

    LightingModel m_lighting = LightingModel::Phong;
    bool m_rendering = false;
    int m_progress{0};
    int m_lastRenderTime{0};
    ShapeQmlBridge m_selectedObject{this};

    QFutureWatcher<void> m_futureWatcher;
    QFutureWatcher<void> m_materialPreviewFutureWatcher;
    QElapsedTimer m_timer;

    // Material preview
    Canvas m_previewCanvas{140, 140};
    Camera m_previewCamera{140, 140, M_PI / 3.0};
    QImage m_previewframebuffer{140, 140, QImage::Format_RGB32};
};

#endif // RAYTRACER_H
