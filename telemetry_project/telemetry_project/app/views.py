from django.contrib.auth import login, authenticate, logout
from django.shortcuts import render, redirect

def devices(request):
    context = {}

    return render(request, 'app/devices.html', context)

def track(request, pck):
    context = {'device_id' : pck}

    return render(request, 'app/device.html', context)

def about_us(request):
    context = {}

    return render(request, 'app/about-us.html', context)

def code_circuit(request):
    context = {}

    return render(request, 'app/code-circuit.html', context)

def detailed(request, pck):
    context = {'device_id' : pck}

    return render(request, 'app/detailed.html', context)

def test(request, pck):
    context = {'device_id' : pck}

    return render(request, 'app/test.html', context)

def track_map(request, pck):
    context = {'device_id' : pck}

    return render(request, 'app/map.html', context)

def admin_login(request):
    context = {}

    if request.method == 'POST':
        user = authenticate(request, username = request.POST.get('username'), password = request.POST.get('password'))

        if user:
            print('hey')
            login(request, user)

            return redirect('devices')

    return render(request, 'app/admin-login.html', context)

def logout_view(request):
    logout(request)

    return redirect('devices')