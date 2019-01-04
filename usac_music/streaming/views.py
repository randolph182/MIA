from django.shortcuts import render, redirect, render_to_response

from django.template import RequestContext
from django.http import HttpResponse

from streaming.forms import form_login,form_registro,form_registro2,form_csv
from django.contrib import messages
from streaming.models import Usuario
from django.db import connection

from django.http import HttpResponseRedirect

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
			cursor = connection.cursor()
			cursor.execute("SELECT id_usuario,rol FROM usuario WHERE nombre = '"+nombre_usr+"' AND password = '"+passw+"';")
			usr = cursor.fetchone()

			if usr != None:
				id_usr = usr[0]
				rol_usr = usr[1]
				print(id_usr)
				print(rol_usr)
				cursor.close()
				request.session['id_usr'] = id_usr
				request.session['rol_usr'] = rol_usr
				request.session['name_usr'] = nombre_usr
				if rol_usr == 'administrador':
					return render(request,'usuario/administrador/administrador.html')
				messages.success(request, 'Logeado exitosamente')
			else:
				messages.warning(request, 'El usuario o la contrasenia son incorrectos')
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

def logout(request):
	request.session['id_usr'] = ""
	request.session['rol_usr'] = 0
	request.session['name_usr'] = ""
	return render(request, 'inicio/home.html')

def administrador(request):
	return render(request,'usuario/administrador/administrador.html')

def archivoCSV(request):
	if request.method == 'POST' :
		csv_form = form_csv(request.POST, request.FILES)
		if csv_form.is_valid():
			f = request.FILES['file'] #obtenemos los datos del archivo
			i = 0   #iterador de lineas
			for chunk in f.chunks():
				lineas = chunk.split('\n')
				for linea in lineas:
					if i != 0:
						elementos = linea.split(',')
						num_elm = 0
						for elemento in elementos:
							print(str(num_elm) + ": "+ elemento)
							num_elm = num_elm + 1
					i = i + 1
			print("numero de lineas del doc  = ")
			print(i)
			# return HttpResponseRedirect('/success/url/')
		else:
			messages.warning(request, 'Hay problemas para subir el archivo')
	else:
		csv_form = form_csv()
	return render(request, 'usuario/administrador/cargaArchivo.html',{'csv_form':csv_form})
