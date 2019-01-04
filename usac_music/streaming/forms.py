from __future__ import unicode_literals
from __future__ import absolute_import

from django import forms
from streaming.models import Usuario,Pais

class form_login(forms.ModelForm):
    class Meta:
        model = Usuario
        
        fields = [
            'nombre',
            'password',
        ]

        labels = {
            'nombre': 'usuario',
            'password': 'password',
        }
        
        widgets = {
            'nombre':forms.TextInput(),
            'password': forms.PasswordInput(),
        }

class form_registro2(forms.Form):
    nombre = forms.CharField(label='nombre', max_length=100)
    apellidos = forms.CharField(label='apellidos', max_length=100)
    password = forms.CharField(widget=forms.PasswordInput)
    correo = forms.EmailField(label='correo', max_length=100)
    telefono = forms.IntegerField(label='telefono')
    fotografia = forms.CharField(label='fotografia', max_length=100)
    genero = forms.CharField(label='genero', max_length=100)
    fecha_nacimiento =  forms.TimeField(label = 'fecha de nacimiento')
    fecha_registro = forms.TimeField(label = 'fecha de registro')
    direccion = forms.CharField(label='direccion', max_length=100)
    rol = forms.CharField(label='rol', max_length=100)
    pais = forms.CharField(label='pais', max_length=100)
    widgets = {
            'password': forms.PasswordInput(),
        }

class form_csv(forms.Form):
    file = forms.FileField( label='Selecciona el archivo csv')


class form_registro(forms.ModelForm):
    class Meta:
        model = Usuario

        fields = [
            'nombre',
            'apellidos',
            'password',
            'correo',
            'telefono',
            'fotografia',
            'genero',
            'fecha_nacimiento',
            'fecha_registro',
            'direccion',
            'rol',
            'pais_id_pais',
        ]

        labels = {
            'nombre': 'nombre',
            'apellidos': 'apellidos',
            'password': 'password',
            'correo':  'correo',
            'telefono': 'telefono',
            'fotografia': 'fotografia',
            'genero': 'genero',
            'fecha_nacimiento': 'fecha_nacimiento',
            'fecha_registro': 'fecha_registro', 
            'direccion':  'direccion',
            'rol': 'rol', 
            'pais_id_pais': 'pais',
        }

        widgets = {
            'nombre': forms.TextInput(),
            'apellidos': forms.TextInput(),
            'password': forms.PasswordInput(),
            'correo': forms.TextInput(),
            'telefono': forms.TextInput(),
            'fotografia': forms.TextInput(),
            'genero': forms.TextInput(),
            'fecha_nacimiento': forms.TextInput(),
            'fecha_registro': forms.TextInput(),
            'direccion': forms.TextInput(),
            'rol': forms.TextInput(),
            'pais_id_pais' :forms.TextInput(),
        }
