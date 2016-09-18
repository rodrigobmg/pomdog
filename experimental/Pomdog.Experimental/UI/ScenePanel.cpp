// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "ScenePanel.hpp"
#include "PointerPoint.hpp"
#include "UIEventDispatcher.hpp"
#include "Pomdog.Experimental/Gameplay2D/Transform.hpp"
#include "Pomdog.Experimental/Gameplay2D/CameraComponent.hpp"

namespace Pomdog {
namespace UI {
namespace {
//
//Radian<float> SampleTumbleGesture(Vector2 const& position, Vector2 & startPosition, Rectangle const& viewportSize)
//{
//    constexpr float threshold = 1.0f;
//
//    Radian<float> delta = 0.0f;
//
//    auto const distance = Vector2::Distance(startPosition, position);
//
//    if (threshold < distance)
//    {
//        auto const centerPoint = viewportSize.GetCenter();
//        Vector2 const screenOffset(centerPoint.X, centerPoint.Y);
//
//        auto vecInScreen1 = (startPosition - screenOffset);
//        auto vecInScreen2 = (position - screenOffset);
//        auto const cross = Vector2::Cross(vecInScreen2, vecInScreen1);
//
//        if (std::abs(cross) > 1.0f)
//        {
//            auto const cosAngle = Vector2::Dot(vecInScreen1, vecInScreen2)/(vecInScreen1.Length() * vecInScreen2.Length());
//            delta = std::acos(MathHelper::Clamp(cosAngle, -1.0f, 1.0f)) * (cross > 0.0f ? 1.0f: -1.0f);
//            POMDOG_ASSERT(delta != std::numeric_limits<float>::signaling_NaN());
//
//            if ((vecInScreen1.Length() < 24.0f || vecInScreen2.Length() < 24.0f)) {
//                delta *= std::min(std::abs(cross/vecInScreen2.Length())*0.01f, 1.0f);
//            }
//
//            startPosition = std::move(position);
//        }
//    }
//
//    return delta;
//}
//
//Vector2 SampleTrackGesture(Vector2 const& position, Vector2 & startPosition)
//{
//    constexpr float threshold = 2.0f;
//
//    Vector2 delta = Vector2::Zero;
//
//    if (threshold < Vector2::Distance(startPosition, position)) {
//        delta = position - startPosition;
//        startPosition = position;
//    }
//
//    return delta;
//}

static const auto ZoomAnimationInterval = std::chrono::milliseconds(400);

} // unnamed namespace

ScenePanel::ScenePanel(
    std::shared_ptr<UIEventDispatcher> const& dispatcher,
    int widthIn,
    int heightIn)
    : UIElement(dispatcher)
    , timer(Duration::zero())
    , normalizedScrollDirection(0.0f)
    , scrollAcceleration(0.0f)
    , cameraZoom(1.0)
    , isFocused(false)
    , isEnabled(true)
{
    SetSize(widthIn, heightIn);
    DrawOrder(10000);
}

// MARK: - Properties

bool ScenePanel::IsEnabled() const
{
    return isEnabled;
}

void ScenePanel::IsEnabled(bool isEnabledIn)
{
    this->isEnabled = isEnabledIn;
}

bool ScenePanel::IsFocused() const
{
    return this->isFocused;
}

// MARK: - Member Functions

Vector2 ScenePanel::ConvertToPanelSpace(Point2D const& point) const
{
    return Vector2(point.X, Height() - point.Y);
}

void ScenePanel::OnEnter()
{
    auto dispatcher = Dispatcher();
    connection = dispatcher->Connect(shared_from_this());
}

void ScenePanel::OnPointerWheelChanged(PointerPoint const& pointerPoint)
{
    scrollWheelSampler.AddWheelDelta(pointerPoint.MouseWheelDelta);
    auto wheelDeltaUnit = scrollWheelSampler.GetScrollWheelDeltaAverage();

    constexpr float accel = 1.2f;

    if (timer <= Duration::zero()) {
        timer = ZoomAnimationInterval;
        scrollAcceleration = accel;
    }
    else {
        scrollAcceleration += std::abs(pointerPoint.MouseWheelDelta / wheelDeltaUnit);
    }

    if (pointerPoint.MouseWheelDelta != 0)
    {
        auto direction = (pointerPoint.MouseWheelDelta >= 0 ? 1.0f : -1.0f);

        if (direction != normalizedScrollDirection) {
            normalizedScrollDirection = direction;
            timer = ZoomAnimationInterval;
            scrollAcceleration = accel;
        }
    }
}

void ScenePanel::OnPointerEntered(PointerPoint const& pointerPoint)
{
    isFocused = true;
}

void ScenePanel::OnPointerExited(PointerPoint const& pointerPoint)
{
    isFocused = false;
}

void ScenePanel::OnPointerPressed(PointerPoint const& pointerPoint)
{
    POMDOG_ASSERT(pointerPoint.MouseEvent);

    switch (*pointerPoint.MouseEvent) {
    case PointerMouseEvent::LeftButtonPressed:
        OnMouseLeftButtonPressed(pointerPoint);
        break;
    case PointerMouseEvent::MiddleButtonPressed:
        OnMouseMiddleButtonPressed(pointerPoint);
        break;
    case PointerMouseEvent::RightButtonPressed:
        OnMouseRightButtonPressed(pointerPoint);
        break;
    default:
        break;
    }
}

void ScenePanel::OnPointerMoved(PointerPoint const& pointerPoint)
{
    POMDOG_ASSERT(pointerPoint.MouseEvent);

    switch (*pointerPoint.MouseEvent) {
    case PointerMouseEvent::LeftButtonPressed:
        OnMouseLeftButtonMoved(pointerPoint);
        break;
    case PointerMouseEvent::MiddleButtonPressed:
        OnMouseMiddleButtonMoved(pointerPoint);
        break;
    case PointerMouseEvent::RightButtonPressed:
        OnMouseRightButtonMoved(pointerPoint);
        break;
    default:
        break;
    }
}

void ScenePanel::OnPointerReleased(PointerPoint const& pointerPoint)
{
    tumbleStartPosition = Pomdog::NullOpt;
    trackStartPosition = Pomdog::NullOpt;
}

void ScenePanel::OnMouseLeftButtonPressed(PointerPoint const& pointerPoint)
{
    if (!tumbleStartPosition) {
        tumbleStartPosition = ConvertToPanelSpace(pointerPoint.Position);
    }
}

void ScenePanel::OnMouseLeftButtonMoved(PointerPoint const& pointerPoint)
{
    if (!isEnabled) {
        return;
    }

    if (!tumbleStartPosition) {
        return;
    }

//    POMDOG_ASSERT(cameraObject);
//    auto transform = cameraObject.Component<Transform2D>();
//    auto camera = cameraObject.Component<Camera2D>();
//
//    if (!transform || !camera) {
//        return;
//    }
//
//    {
//        // Tumble Gesture
//        transform->Rotation += SampleTumbleGesture(ConvertToPanelSpace(pointerPoint.Position), *tumbleStartPosition,
//            Rectangle{0, 0, Width(), Height()});
//        if (transform->Rotation > MathConstants<float>::TwoPi()) {
//            transform->Rotation -= MathConstants<float>::TwoPi();
//        }
//        else if (transform->Rotation < -MathConstants<float>::TwoPi()) {
//            transform->Rotation += MathConstants<float>::TwoPi();
//        }
//        POMDOG_ASSERT(transform->Rotation != std::numeric_limits<float>::quiet_NaN());
//        POMDOG_ASSERT(transform->Rotation != std::numeric_limits<float>::infinity());
//    }
}

void ScenePanel::OnMouseMiddleButtonPressed(PointerPoint const& pointerPoint)
{
    trackStartPosition = ConvertToPanelSpace(pointerPoint.Position);
}

void ScenePanel::OnMouseMiddleButtonMoved(PointerPoint const& pointerPoint)
{
    if (!isEnabled) {
        return;
    }

    if (!trackStartPosition) {
        return;
    }

//    POMDOG_ASSERT(cameraObject);
//    auto transform = cameraObject.Component<Transform2D>();
//    auto camera = cameraObject.Component<Camera2D>();
//
//    if (!transform || !camera) {
//        return;
//    }
//
//    // Track Gesture
//    auto delta = SampleTrackGesture(ConvertToPanelSpace(pointerPoint.Position), *trackStartPosition);
//
//    POMDOG_ASSERT(camera->Zoom > 0);
//    auto matrix = (Matrix3x3::CreateTranslation(delta)
//        * Matrix3x3::CreateScale(1.0f / camera->Zoom)
//        * Matrix3x3::CreateRotationZ(transform->Rotation));
//
//    transform->Position -= {matrix(2, 0), matrix(2, 1)};
}

void ScenePanel::OnMouseRightButtonPressed(PointerPoint const& pointerPoint)
{
    SceneTouch(ConvertToPanelSpace(pointerPoint.Position));
}

void ScenePanel::OnMouseRightButtonMoved(PointerPoint const& pointerPoint)
{
    SceneTouch(ConvertToPanelSpace(pointerPoint.Position));
}

void ScenePanel::UpdateAnimation(Duration const& frameDuration)
{
    timer = std::max(timer - frameDuration, Duration::zero());

    if (timer <= Duration::zero()) {
        scrollAcceleration = 1.0f;
        return;
    }

    const auto duration = std::min<double>(frameDuration.count(), 1.0);
    const auto scroll = duration
        * normalizedScrollDirection
        * scrollAcceleration
        * (timer.count() / ZoomAnimationInterval.count());

    POMDOG_ASSERT(cameraZoom > 0);
    cameraZoom = MathHelper::Saturate(cameraZoom + (cameraZoom * scroll * 1000));
}

double ScenePanel::GetScrollWheel() const
{
    return cameraZoom;
}

void ScenePanel::OnRenderSizeChanged(int widthIn, int heightIn)
{
    SetSize(widthIn, heightIn);
}

void ScenePanel::Draw(DrawingContext &)
{
}

} // namespace UI
} // namespace Pomdog
