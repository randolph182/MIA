from django.shortcuts import render, redirect, render_to_response

from django.template import RequestContext
from django.http import HttpResponse

from streaming.forms import form_login,form_registro,form_registro2
from django.contrib import messages
from streaming.models import Usuario
from django.db import connection

# Create your views here.
def home(request):
	template = 'inicio/home.html'
	context = {

	}
	return render(request, template, context)

def index(request):
	obj = Usuario.objects.all()
	for abc in obj:
		obj_nombre = abc.nombre
		obj_apellidos = abc.obj_apellidos
	
	context = {
		"obj":obj,
		"obj_nombre": obj_nombre,
		"obj_apellido": obj_apellidos
	}
	return render(request,"usuario/index.html",context)
    


def login(request):
	if request.method == 'POST' :
		login_form = form_login(request.POST)

		if login_form.is_valid:
			nombre_usr = request.POST['nombre']
			passw = request.POST['password']
			try:
				verificacion = Usuario.objects.filter(nombre=nombre_usr, password=passw).exists()
				if verificacion == True:
					messages.success(request, 'Logeado exitosamente') 
					print(nombre_usr)
					print(passw)
				else:
					messages.warning(request, 'El usuario o la contrasenia son incorrectos') 
			except Exception:
				messages.warning(request, 'Algo salio mal en la validacion de usuarios') 
	else:
		login_form = form_login()
	return render(request, 'usuario/login.html',{'login_form':login_form})

def registro_usr(request):
	if request.method == 'POST' :
		registro_form = form_registro2(request.POST)

		if registro_form.is_valid:
			name = request.POST['nombre']
			passw = request.POST['password']
			apell = request.POST['apellidos']
			correo = request.POST['correo']
			tel = request.POST['telefono']
			foto = request.POST['fotografia']
			genero = request.POST['genero']
			fecha_nac = request.POST['fecha_nacimiento']
			fecha_reg = request.POST['fecha_registro']
			dire = request.POST['direccion']
			rol = request.POST['rol']
			pais = request.POST['pais']
			print(name)
			print(pais)
			print(fecha_nac)
			acum = 'execute registroUsuario(\''+ name +'\',\''+apell+'\',\''+passw+'\',\''+correo+'\','+ tel +',\''+foto+'\',\''+genero+'\',TO_DATE(\''+fecha_nac+'\',\'yyyy/mm/dd\'),TO_DATE(\''+fecha_reg+'\',\'yyyy/mm/dd\'),\''+dire+'\',\''+rol+'\',\''+pais+'\');'
			print(acum)
			with connection.cursor() as cursor:
				cursor.callproc("registroUsuario",(name,apell,passw,correo,tel,foto,genero,fecha_nac,fecha_reg,dire,rol,pais))
				cursor.close()

	else:
		registro_form = form_registro2()
	return render(request, 'usuario/registro.html',{'registro_form':registro_form})