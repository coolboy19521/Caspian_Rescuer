from .views import getRoutes, getDevices, getLog, getLatest, servoControl, motorControl, manualControl, autonomusControl
from django.urls import path

urlpatterns = [
    path('', getRoutes),
    path('devices/', getDevices),
    path('getLog/<int:pck>', getLog),
    path('getLatest/<int:pck>', getLatest),
    path('servoControl/', servoControl),
    path('motorControl/', motorControl),
    path('manualControl/', manualControl),
    path('autonomusControl/', autonomusControl)
]
