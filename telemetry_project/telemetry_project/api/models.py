from django.db import models

class Device(models.Model):
    date = models.DateField()
    time = models.TimeField()
    device_id = models.IntegerField()
    gps = models.BooleanField()
    gps_find_satellite = models.BooleanField()

    device_name = models.CharField(max_length = 255)