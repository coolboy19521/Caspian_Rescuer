from .views import devices, track, about_us, code_circuit, detailed, test, track_map, admin_login, logout_view
from django.urls import path

urlpatterns = [
    path('devices/', devices, name = 'devices'),
    path('logout/', logout_view, name = 'logout'),
    path('track/<int:pck>', track, name = 'track'),
    path('detailed/<int:pck>', detailed, name = 'detailed'),
    path('test/<int:pck>', test, name = 'test'),
    path('about_us/', about_us, name = 'about_us'),
    path('track_map/<int:pck>', track_map, name = 'track_map'),
    path('admin_login/', admin_login, name = 'admin_login'),
    path('code_circuit/', code_circuit, name = 'code_circuit')
]