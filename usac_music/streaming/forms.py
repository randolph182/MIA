from __future__ import unicode_literals
from __future__ import absolute_import

from django import forms
from streaming.models import Usuario,Pais

class form_login(forms.ModelForm):
    class Meta:
        model = Usuario
        
        fields = [
            'correo',
            'password',
        ]

        labels = {
            'correo': 'Email',
            'password': 'password',
        }
        
        widgets = {
            'correo':forms.TextInput(),
            'password': forms.PasswordInput(),
        }

class form_registro2(forms.Form):
    nombre = forms.CharField(label='nombre', max_length=100)
    apellidos = forms.CharField(label='apellidos', max_length=100)
    password = forms.CharField(widget=forms.PasswordInput)
    correo = forms.EmailField(label='correo', max_length=100)
    telefono = forms.IntegerField(label='telefono')
    fotografia = forms.FileField(required=False,label='fotografia')
    genero = forms.CharField(label='genero', max_length=100)
    fecha_nacimiento =  forms.CharField(label = 'fecha de nacimiento')
    direccion = forms.CharField(label='direccion', max_length=100)
    rol = forms.CharField(label='rol', max_length=100)
    pais = forms.CharField(label='pais', max_length=100)
    widgets = {
            'password': forms.PasswordInput(),
        }

class form_csv(forms.Form):
    file = forms.FileField( label='Selecciona el archivo csv')


class form_registro(forms.Form):
    nombre = forms.CharField(label='nombre', max_length=100)
    apellidos = forms.CharField(label='apellidos', max_length=100)
    password = forms.CharField(widget=forms.PasswordInput)
    correo = forms.EmailField(label='correo', max_length=100)
    telefono = forms.IntegerField(label='telefono')
    fotografia = forms.FileField(required=False,label='fotografia')
    genero = forms.CharField(label='genero', max_length=100)
    fecha_nacimiento =  forms.CharField(label = 'fecha de nacimiento')
    direccion = forms.CharField(label='direccion', max_length=100)
    pais = forms.CharField(label='pais', max_length=100)
    widgets = {
            'password': forms.PasswordInput(),
        }

class form_correo(forms.Form):
    correo_id = forms.EmailField(label='correo mod', max_length=100)


class form_datosUsr(forms.Form):
    nombre = forms.CharField(label='nombre', max_length=100)
    apellidos = forms.CharField(label='apellidos', max_length=100)
    password = forms.CharField(label='password', max_length=100)
    correo = forms.EmailField(label='correo', max_length=100)
    telefono = forms.IntegerField(label='telefono')
    fotografia = forms.FileField(required=False,label='fotografia')
    genero = forms.CharField(label='genero', max_length=100)
    fecha_nacimiento =  forms.CharField(label = 'fecha de nacimiento')
    direccion = forms.CharField(label='direccion', max_length=100)
    rol = forms.CharField(label='rol', max_length=100)
    pais = forms.CharField(label='pais', max_length=100)
    comentario = forms.CharField(widget=forms.Textarea)
    c_activo = forms.IntegerField(label='Cuenta activa')

class form_delUsr(forms.Form):
    correo = forms.EmailField(label='correo', max_length=100)
    comentario = forms.CharField(widget=forms.Textarea)