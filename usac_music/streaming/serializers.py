from rest_framework import serializers
from .models import Cancion


class CancionSerializer(serializers.HyperlinkedModelSerializer):
	class Meta:
		model= Cancion
		fields=('id_cancion','nombre','ruta_cancion')