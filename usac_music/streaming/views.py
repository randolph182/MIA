from django.shortcuts import render, redirect, render_to_response

from django.template import RequestContext
from django.http import HttpResponse

from streaming.forms import form_login,form_registro,form_registro2,form_csv
from django.contrib import messages
from streaming.models import Usuario
from django.db import connection
import cx_Oracle
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
	return render(request, 'usuario/administrador/adminRegistroUsr.html',{'registro_form':registro_form})



def registro_normal(request):
	if request.method == 'POST' :
		registro_form = form_registro(request.POST)
		if registro_form.is_valid:
			name = request.POST['nombre']
			passw = request.POST['password']
			apell = request.POST['apellidos']
			correo = request.POST['correo']
			tel = request.POST['telefono']
			foto = request.POST['fotografia']
			genero = request.POST['genero']
			fecha_nac = request.POST['fecha_nacimiento']
			fecha_reg = "2019/01/05"
			dire = request.POST['direccion']
			rol = "usuario"
			pais = request.POST['pais']
			resultado = 0
			print(name)
			print(pais)
			print(fecha_nac)
			# acum = ' DECLARE result number;BEGIN registroUsuario(\''+ name +'\',\''+apell+'\',\''+passw+'\',\''+correo+'\','+ tel +',\''+foto+'\',\''+genero+'\',TO_DATE(\''+fecha_nac+'\',\'yyyy/mm/dd\'),TO_DATE(\''+fecha_reg+'\',\'yyyy/mm/dd\'),\''+dire+'\',\''+rol+'\',\''+pais+'\');'
			acum = """BEGIN
   						REGISTROUSUARIO('usuario6','usuario6,','123456','usuario6@gmail.com',123456,'home','masculino',TO_DATE('2019/01/01','yyyy/mm/dd'),TO_DATE('2019/01/01','yyyy/mm/dd'),'guate','usuario','Guatemala');
				      END;;"""
			print(acum)
			# try:
			# 	plsql="registroUsuario(%s, %s, %s,%s, %s, %s,%s, %s, %s,%s, %s, %s)"
			# 	resultado = call_procedimiento(plsql, [name,apell,passw,correo,tel,foto,genero,fecha_nac,fecha_reg,dire,rol,pais])
			# 	print(resultado)
			# except Exception, ex:
			# 	print(str(ex))
			# with connection.cursor() as cursor:
			# 	respuesta = cursor.callproc("registroUsuario",(name,apell,passw,correo,tel,foto,genero,fecha_nac,fecha_reg,dire,rol,pais,resultado))
			# 	# respuesta = cursor.fetchone()
			# 	print(resultado)
			# 	cursor.close()
			with connection.cursor() as cursor:
				cursor.execute(acum)
				result = cursor.fetchone()
				print(result)
				cursor.close()
			
	else:
		registro_form = form_registro()
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
						#inicializamos las variables por defecto por si no se encuntra algun campo
						if linea != "":
							nombre_song = ""
							fecha_song = "2019/01/01"
							genero_song = "----"
							nombre_artista = ""
							nombre_album = ""
							fecha_album = "2019/01/01"
							genero_album = ""
							fecha_artista = "2019/01/01"
							pais_artista = ""
							ruta_cancion = "---------"
							#epliteamos los campos por coma
							elementos = linea.split(',')
							#asignamos los valores a las variables
							nombre_song = elementos[0]
							fecha_song = elementos[1]
							genero_song = elementos[2]
							nombre_artista = elementos[3]
							nombre_album = elementos[4]
							fecha_album = elementos[5]
							genero_album = elementos[6]
							fecha_artista = elementos[7]
							pais_artista = elementos[8]
							#ejecutamos el procedimiento almacenado que esta en oracle
							with connection.cursor() as cursor:
								cursor.callproc("archivoCSV",(nombre_song,fecha_song,genero_song,nombre_artista,nombre_album,fecha_album,genero_album,fecha_artista,pais_artista,ruta_cancion))
								cursor.close()
					i = i + 1
			messages.warning(request, 'El archivo se ha subido')
		else:
			messages.warning(request, 'Hay problemas para subir el archivo')
	else:
		csv_form = form_csv()
	return render(request, 'usuario/administrador/cargaArchivo.html',{'csv_form':csv_form})

def call_procedimiento(cadena, valors=None):  
	with connection.cursor() as cur:
		# preparam cadena per a Oracle
		cadena="BEGIN %s; END;;" % (cadena,)
		# preparam el parametre de sortida
		sortida= cur.cursor.var(cx_Oracle.STRING)
		# afeixim parAmetre com a darrer
		valors.append(sortida)
		try:
			cur.execute(cadena, valors)
			# retornam parAmetre de sortida
			return sortida.getvalue()
		except Exception, e:
			mensaje="%s Error: call_proc cadena#%s valors#%s" % (e, cadena, valors)
			raise Exception(mensaje)
		finally:
			cur.close()