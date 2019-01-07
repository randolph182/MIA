from django.shortcuts import render, redirect, render_to_response

from django.template import RequestContext
from django.http import HttpResponse

from streaming.forms import form_login,form_registro,form_registro2,form_csv,form_correo,form_datosUsr,form_delUsr
from django.contrib import messages
from streaming.models import Usuario
from django.db import connection
from django.http import HttpResponseRedirect

from django.core.mail import EmailMessage
import random, string

import os

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
    

#el usuario se logeara por medio del correo
def login(request):
	if request.method == 'POST' :
		login_form = form_login(request.POST)

		if login_form.is_valid:
			correo_usr = request.POST['correo']
			passw = request.POST['password']
			cursor = connection.cursor()
			cursor.execute("SELECT id_usuario,rol,nombre FROM usuario WHERE correo = '"+correo_usr+"' AND password = '"+passw+"' AND tactivo = 1;")
			usr = cursor.fetchone()

			if usr != None:
				id_usr = usr[0]
				rol_usr = usr[1]
				print(id_usr)
				print(rol_usr)
				cursor.close()
				request.session['id_usr'] = id_usr
				request.session['rol_usr'] = rol_usr
				request.session['name_usr'] = usr[2]
				if rol_usr == 'administrador':
					return render(request,'usuario/administrador/principalAdmin.html')
				messages.success(request, 'Logeado exitosamente')
			else:
				messages.warning(request, 'El usuario o la contrasenia son incorrectos o no esta activa la cuenta')
	else:
		login_form = form_login()
	return render(request, 'usuario/login.html',{'login_form':login_form})

def registro_usr(request):
	if request.method == 'POST' :
		registro_form = form_registro2(request.POST,request.FILES or None)

		if registro_form.is_valid():
			name = request.POST['nombre']
			passw = request.POST['password']
			apell = request.POST['apellidos']
			correo = request.POST['correo']
			tel = request.POST['telefono']
			# foto = request.POST['fotografia']
			genero = request.POST['genero']
			fecha_nac = request.POST['fecha_nacimiento']
			dire = request.POST['direccion']
			rol = request.POST['rol']
			pais = request.POST['pais']
			cActivo = 1
			token = ''
			handle_uploaded_file(request.FILES['fotografia'],str(request.FILES['fotografia']))
			ruta_archivo = '/home/rm/Documentos/Django/MIA/usac_music/upload' + str(request.FILES['fotografia'])
			with connection.cursor() as cursor:
				cursor.callproc("registroUsuario",(name,apell,passw,correo,tel,ruta_archivo,genero,fecha_nac,dire,rol,pais,token,cActivo))
				cursor.close()
	else:
		registro_form = form_registro2()
	return render(request, 'usuario/administrador/adminRegistroUsr.html',{'registro_form':registro_form})



def registro_normal(request):
	if request.method == 'POST' :
		registro_form = form_registro(request.POST,request.FILES or None)
		if registro_form.is_valid():
			name = request.POST['nombre']
			passw = request.POST['password']
			apell = request.POST['apellidos']
			correo = request.POST['correo']
			tel = request.POST['telefono']
			# foto = request.POST['fotografia']
			genero = request.POST['genero']
			fecha_nac = request.POST['fecha_nacimiento']
			dire = request.POST['direccion']
			rol = "usuario"
			pais = request.POST['pais']
			cActivo = 0
			handle_uploaded_file(request.FILES['fotografia'],str(request.FILES['fotografia']))
			ruta_archivo = '/home/rm/Documentos/Django/MIA/usac_music/upload' + str(request.FILES['fotografia'])
			
			llveEmail = random_string(10)

			msg = EmailMessage(
				'DJANGO confirm email'
				'Para activar su cuenta en USAC MUSIC acceda al siguiente enlace: http://127.0.0.1:8000/correo_verificacion/' + llveEmail + '/',
				to=[correo]
			)
			msg.send()
			with connection.cursor() as cursor:
				cursor.callproc("registroUsuario",(name,apell,passw,correo,tel,ruta_archivo,genero,fecha_nac,dire,rol,pais,llveEmail,cActivo))
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
	return render(request,'usuario/administrador/principalAdmin.html')

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

def crud_usuario(request):
	return render(request,'usuario/administrador/crudUsuarios.html')

def crud_show_usr(request):
	usr = Usuario.objects.all()
	contexto = {'usuarios':usr}
	return render(request,'usuario/administrador/mostrarUsuarios.html',contexto)

def crud_modif_usr(request):
	if request.method == 'POST' :
		usr = Usuario.objects.all()
		correo = form_correo(request.POST)
		usrData = form_datosUsr(request.POST, request.FILES or None)
		contexto = {'usuarios':usr,'correo':correo,'datos_usr':usrData}
		#verificando primero el correo 
		if correo.is_valid():
			val_correo = correo.cleaned_data['correo_id']
			result_usr = None
			try:
				result_usr = Usuario.objects.get(correo = val_correo)
			except:
				result_usr = None
			if result_usr: #si el usuario es valido que esxiste
				if usrData.is_valid(): #ahora pregunta si los campos del otro formulario son correctos
					nombre = usrData.cleaned_data['nombre']
					apell = usrData.cleaned_data['apellidos']
					passw = usrData.cleaned_data['password']
					nCorreo = usrData.cleaned_data['correo']
					tel = usrData.cleaned_data['telefono']
					cActivo = usrData.cleaned_data['c_activo']
					#para la foto
					handle_uploaded_file(request.FILES['fotografia'],str(request.FILES['fotografia']))
					foto = '/home/rm/Documentos/Django/MIA/usac_music/upload' + str(request.FILES['fotografia'])
					# foto = usrData.cleaned_data['fotografia']
					genero = usrData.cleaned_data['genero']
					fecha_nac = usrData.cleaned_data['fecha_nacimiento']
					direccion = usrData.cleaned_data['direccion']
					rol = usrData.cleaned_data['rol']
					pais = usrData.cleaned_data['pais']
					coment = usrData.cleaned_data['comentario']
					#luego de obtener todos los datos a modificar se procede a verficiar que el nuevo correo no este rep
					result_correo = None
					try:
						result_correo = Usuario.objects.get(correo = nCorreo)
					except :
						result_correo = None
						
					if result_correo == None: #sino exite el correo se registra
						id1 = request.session['id_usr']
						id2 = result_usr.id_usuario
						with connection.cursor() as cursor:
							cursor.callproc("actualizarUsuario",(id1,id2,nombre,apell,passw,nCorreo,tel,foto,genero,fecha_nac,direccion,rol,pais,coment,cActivo))
							cursor.close()
					elif result_correo != None and result_correo.nombre == nombre: #si el correo existe y es el mismo nombre
						id1 = request.session['id_usr']
						id2 = result_usr.id_usuario
						with connection.cursor() as cursor:
							cursor.callproc("actualizarUsuario",(id1,id2,nombre,apell,passw,nCorreo,tel,foto,genero,fecha_nac,direccion,rol,pais,coment,cActivo))
							cursor.close()
					else:
						print("el correo ya existe al que desea modificar")

				else:
					print("tiene que escribir todos los campos del usuario que desea modificar")
			else:
				print("el correo del usuario que desea modificar no existe")
	else:
		usr = Usuario.objects.all()
		correo = form_correo()
		usrData = form_datosUsr()
		contexto = {'usuarios':usr,'correo':correo,'datos_usr':usrData}
	return render(request,'usuario/administrador/adminModifUsr.html',contexto)


def admin_del_usr(request):
	if request.method == 'POST' :
		form_del = form_delUsr(request.POST)
		if form_del.is_valid():
			nCorreo = form_del.cleaned_data['correo']
			comentario = form_del.cleaned_data['comentario']
			usr = None
			try:
				usr = Usuario.objects.get(correo = nCorreo)
			except :
				usr = None
			if usr != None:
				id1 = request.session['id_usr']
				id2 = usr.id_usuario
				with connection.cursor() as cursor:
					cursor.execute('DELETE FROM usuario WHERE correo = \''+nCorreo+'\';')
					messages.success(request, 'el usuario fue eliminado')
					cursor.close()
				# with connection.cursor() as cursor:
				# 	tipo = "eliminacion"
				# 	cursor.callproc("insertarUsrLog",(comentario,tipo,id1,id2))
				# 	cursor.close();
			else:
				messages.success(request, 'El correo que ingreso no es valido')
		else:
			messages.success(request, 'tiene que llenar todos los campos requeridos')
	else:
		form_del = form_delUsr()
	return render(request,'usuario/administrador/adminDelUsr.html',{'form_del':form_del})

def handle_uploaded_file(file,filename):
	if not os.path.exists('upload/'):
		os.mkdir('upload/')
	with open('upload/'+filename,'wb+') as destino: #ojo es archivo binario
		for chunk in file.chunks():
			destino.write(chunk)

def random_string(cant):
	return ''.join(random.choice(string.ascii_lowercase) for a in range(cant))

def confirm_correo(request,url_str):
	if request.method == 'POST':
		return redirect('home')
	else:
		usr_new = None
		try:
			usr_new = Usuario.objects.get(token_correo = url_str)
		except:
			usr_new = None
		if usr_new != None:
			sql = """UPDATE usuario
					 SET tactivo = 1
					 WHERE id_usuario = """+ str(usr_new.id_usuario)+""";"""	
			cursor = connection.cursor()
			cursor.execute(sql)
			cursor.close()
		else:
			print("HUBO PROBLEMAS CON LA VERIFICACION DEL TOKEN CON EL USUARIO")

	return render(request,'inicio/confirmCorreo.html',{})