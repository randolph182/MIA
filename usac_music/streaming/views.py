from django.shortcuts import render, redirect, render_to_response

from django.template import RequestContext
from django.http import HttpResponse

from streaming.forms import form_login,form_registro,form_registro2,form_csv,form_correo,form_datosUsr,form_delUsr,form_regArti,form_nombre_artMod,form_delArt
from django.contrib import messages
from streaming import forms
from streaming.models import Usuario,Artista,Cancion,Album,AlbmSong,Genero,Membresia,Suscriptor,Pais,Genero,UsrLog
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
				elif rol_usr == 'usuario':
					#debo preguntar si tiene membreia activa
					sql = """SELECT COUNT(*)
							 FROM membresia
							 WHERE usuario_id_usuario = """+ str(id_usr) +"""
							 AND activa = 1;"""
					cursor = connection.cursor()
					cursor.execute(sql)
					tiene_memb = cursor.fetchone()
					cursor.close()
					if tiene_memb != None:
						if tiene_memb[0] > 0 :
							return redirect('usuarioPremium')
						else:
							return render(request,'usuario/normal/principalUsuario.html')
					else:
						messages.success(request, 'Salio algo mal en la consulta')
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
			ruta_archivo = str(request.FILES['fotografia'])
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
			ruta_archivo = str(request.FILES['fotografia'])
			
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
							ruta_cancion = "Pink_Floyd-Comfortably_numb.mp3"
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
					foto = str(request.FILES['fotografia'])
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
							messages.success(request, "datos modificados")
					elif result_correo != None and result_correo.nombre == result_usr.nombre: #si el correo existe y es el mismo nombre
						id1 = request.session['id_usr']
						id2 = result_usr.id_usuario
						with connection.cursor() as cursor:
							cursor.callproc("actualizarUsuario",(id1,id2,nombre,apell,passw,nCorreo,tel,foto,genero,fecha_nac,direccion,rol,pais,coment,cActivo))
							cursor.close()
							messages.success(request, "datos modificados")
					else:
						messages.success(request, "el correo ya existe al que desea modificar")

				else:
					messages.success(request, "tiene que escribir todos los campos del usuario que desea modificar")
			else:
				messages.success(request, "el correo del usuario que desea modificar no existe")
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


def crud_artista(request):
	return render(request,'usuario/administrador/crudArtistas.html')

def crud_show_artistas(request):
	art = Artista.objects.all()
	contexto = {'artistas':art}
	return render(request,'usuario/administrador/mostrarArtistas.html',contexto)

def crud_reg_art(request):
	if request.method == 'POST':
		form = form_regArti(request.POST)
		if form.is_valid():
			#VERIFICANDO PRIMERO SINO EXISTE EL ARTISTA
			nombre = form.cleaned_data['nombre']
			result = None
			try:
				result = Artista.objects.get(nombre = nombre)
			except:
				result = None

			if result == None: #signifia que no existe
				fecha_nac = form.cleaned_data['fecha_nacimiento']
				pais = form.cleaned_data['pais']
				handle_uploaded_file(request.FILES['fotografia'],str(request.FILES['fotografia']))
				foto = str(request.FILES['fotografia'])
				with connection.cursor() as cursor:
					cursor.callproc("registroArtista",(nombre,fecha_nac,foto,pais))
					cursor.close()
					messages.success(request, 'Se ha registrado un nuevo Artista')
			else:
				messages.success(request, 'el artista que desea registrar ya existe')
	else:
		form = form_regArti()
	return render(request,'usuario/administrador/adminRegistroArt.html',{'formArt':form})


def crud_mod_art(request):
	art = Artista.objects.all()
	if request.method == 'POST':
		nomb_viejo = form_nombre_artMod(request.POST)
		form = form_regArti(request.POST,request.FILES or None)
		contexto = {'artistas':art,'nomb_viejo':nomb_viejo,'campos':form}
		#verificando el nombre viejo
		if nomb_viejo.is_valid():
			old_name = nomb_viejo.cleaned_data['nombre_viejo']
			#verificamos si existe en nombre que queremos modifica
			print(old_name)
			artista_actual = None
			try:
				artista_actual = Artista.objects.get(nombre = old_name)
			except:
				artista_actual = None

			if artista_actual != None: #si existe el usuario
				#preguntamos si se llenaron los campos del formulario para modificar al artista
				if form.is_valid():
					nombre = form.cleaned_data['nombre']
					#verificando que el nuevo nombre no sea igual a los demas artistas o que si sea del mismo artista
					mod_art = None
					try:
						mod_art = Artista.objects.get(nombre = nombre)
					except:
						mod_art = None

					fecha_nac = form.cleaned_data['fecha_nacimiento']
					pais = form.cleaned_data['pais']
					id_art_act = artista_actual.id_artista
					handle_uploaded_file(request.FILES['fotografia'],str(request.FILES['fotografia']))
					foto = str(request.FILES['fotografia'])

					if mod_art == None: #no existe el artista
						with connection.cursor() as cursor:
							cursor.callproc("actualizarArtista",(id_art_act,nombre,fecha_nac,foto,pais))
							cursor.close()
							messages.success(request, "datos modificados")
					elif mod_art != None and mod_art.nombre == artista_actual.nombre:
						with connection.cursor() as cursor:
							cursor.callproc("actualizarArtista",(id_art_act,nombre,fecha_nac,foto,pais))
							cursor.close()
							messages.success(request, "datos modificados")
					else:
						messages.success(request, "el nombre porque el que lo desa cambiar ya existe")
				else:
					messages.success(request, 'asegurese que estan llenos los campos para modifiar al artista')
			else:
				messages.success(request, 'el artista que desea modificar no existe en la base de datos')

		else:
			messages.success(request, 'tiene que llenar el campo del nombre viejo')
	else:	
		nomb_viejo = form_nombre_artMod()
		form = form_regArti()
		contexto = {'artistas':art,'nomb_viejo':nomb_viejo,'campos':form}
	return render(request,'usuario/administrador/adminModArt.html',contexto)



def admin_del_art(request):
	if request.method == 'POST' :
		form_del = form_delArt(request.POST)
		if form_del.is_valid():
			nomb = form_del.cleaned_data['nombre']
			art = None
			try:
				art = Artista.objects.get(nombre = nomb)
			except :
				art = None
			if art != None:
				with connection.cursor() as cursor:
					cursor.execute('DELETE FROM artista WHERE nombre = \''+nomb+'\';')
					messages.success(request, 'el artista fue eliminado')
					cursor.close()
			else:
				messages.success(request, 'El artista que ingreso no es valido')
		else:
			messages.success(request, 'tiene que llenar todos los campos requeridos')
	else:
		form_del = form_delArt()
	return render(request,'usuario/administrador/adminDelArt.html',{'form_del':form_del})


def crud_canciones(request):
	return render(request,'usuario/administrador/crudCanciones.html')

def crud_show_canciones(request):
	canciones = AlbmSong.objects.all()
	return render(request,'usuario/administrador/mostrarCanciones.html',{'canciones':canciones})


def crud_mod_canciones(request):
	canciones = Cancion.objects.all()
	if request.method == 'POST':
		nombCan = forms.form_nombCAn(request.POST)
		infoCan = forms.form_modCan(request.POST)
		if nombCan.is_valid():
			nomb_act_can = nombCan.cleaned_data['nombre']

			cancion_actual = None
			try:
				cancion_actual = Cancion.objects.get(nombre = nomb_act_can)
			except :
				cancion_actual = None

			if cancion_actual != None:
				if infoCan.is_valid():

					nomb_genero = infoCan.cleaned_data['genero']
					nomb_artista = infoCan.cleaned_data['artista']

					nuevo_genero = None
					nuevo_artista = None
					try:
						nuevo_genero = Genero.objects.get(nombre = nomb_genero)
						nuevo_artista = Artista.objects.get(nombre = nomb_artista)
					except :
						nuevo_genero = None
						nuevo_artista = None

					if nuevo_genero != None:
						if nuevo_artista != None:
							nuevo_nomb = infoCan.cleaned_data['nombre']

							cancion_nueva = None
							try:
								cancion_nueva = Cancion.objects.get(nombre = nuevo_nomb)
							except :
								cancion_nueva = None

							idGenero = nuevo_genero.id_genero
							idArtista = nuevo_artista.id_artista
							fecha_lanzamiento = infoCan.cleaned_data['fecha_lanzamiento']
							idCan = cancion_nueva.id_cancion
							handle_uploaded_file(request.FILES['ruta_cancion'],str(request.FILES['ruta_cancion']))
							ruta_cancion = str(request.FILES['ruta_cancion'])

							if cancion_nueva == None:
								with connection.cursor() as cursor:
									cursor.callproc("modificarCancion",(idCan,nuevo_nomb,fecha_lanzamiento,idGenero,idArtista,ruta_cancion))
									messages.success(request, 'la cancion fue modificada')
									cursor.close()
							elif cancion_nueva != None and cancion_nueva.nombre == cancion_actual.nombre:
								with connection.cursor() as cursor:
									cursor.callproc("modificarCancion",(idCan,nuevo_nomb,fecha_lanzamiento,idGenero,idArtista,ruta_cancion))
									messages.success(request, 'la cancion fue modificada')
									cursor.close()
							else:
								messages.success(request, "el nuevo nobre ya existe")
						else:
							messages.success(request, "el artista no existe tiene que crearlo")
					else:
						messages.success(request, "el genero no existe tiene que crearlo")
				else:
					messages.success(request, "tiene que llenar los campos para modificar la cancion")
			else:
				messages.success(request, "el nombre viejo de la cancion no existe ")

		else:
			messages.success(request, "tiene que llenar el nombre viejo para modificar")
	else:
		nombCan = forms.form_nombCAn()
		infoCan = forms.form_modCan()
	return render(request,'usuario/administrador/adminModCan.html',{'canciones':canciones,'nombre_viejo':nombCan,'form':infoCan})

def show_usr_membresia(request,id_memb):
	memb = Membresia.objects.all()
	if id_memb != '0':
		sql = """UPDATE membresia
				 SET activa = 0	
				 WHERE id_membresia = """+str(id_memb)+""";"""
		cursor = connection.cursor()
		cursor.execute(sql)
		cursor.close()
	return render(request,'usuario/administrador/showUsrMemb.html',{'memb':memb})

#================================================================ USUARIO NORMAL =====================================


def usuarioNormal(request):
	return render(request,'usuario/normal/principalUsuario.html')

def un_show_songs(request):
	sngs = AlbmSong.objects.all()
	return render(request,'usuario/normal/showCanciones.html',{'canciones':sngs})

def play_songs(request,id_song):
	#busacando la cancion por id
	idSng = None
	try:
		idSng = Cancion.objects.get(id_cancion = id_song)
	except:
		idSng = None

	if idSng != None:
		sql = """INSERT INTO play(usuario_id_usuario,cancion_id_cancion)
				 VALUES("""+str(request.session['id_usr'])+""","""+str(idSng.id_cancion)+""");"""
		with connection.cursor() as cursor:
			cursor.execute(sql)
			cursor.close()
		# sngs = Cancion.objects.all()
		return render(request,'playCancion.html',{'cancion':idSng})



def buy_membresia(request):
	if request.method == 'POST':
		formulario = forms.form_newMembresia(request.POST)
		if formulario.is_valid():
			idUsuario = request.session['id_usr']
			noTarjeta = formulario.cleaned_data['tarjeta']
			valorMembresia = 15
			with connection.cursor() as cursor:
				cursor.callproc("nuevaMembresia",(idUsuario,noTarjeta,valorMembresia))
				cursor.close()
				return redirect('home')
		else:
			messages.success(request, "tine que ingresar el Numero de su tarjeta")
	else:
		formulario = forms.form_newMembresia();
	return render(request,'usuario/normal/compraMembresia.html',{'form':formulario})


#================================================================ USUARIO premium =====================================

def usuarioPremium(request):
	return render(request,'usuario/premium/principal.html')

def un_show_songsPrem(request):
	sngs = AlbmSong.objects.all()
	return render(request,'usuario/premium/showCanPrem.html',{'canciones':sngs})

def show_artistas(request,id_art):
	art = Artista.objects.all()
	if id_art != '0':
		idUsr = request.session['id_usr']
		sql = """INSERT INTO suscriptor(usuario_id_usuario,artista_id_artista)
				 VALUES("""+str(idUsr)+""","""+str(id_art)+""");"""
		cursor = connection.cursor()
		cursor.execute(sql)
		cursor.close()
		print(sql)
	else:
		print(id_art)
	return render(request,'usuario/premium/showArtistasPrem.html',{'artistas':art})




	#================================================ ADMIN REPORTES ===============================
def show_repoAdmin(request):
	return render(request,'usuario/administrador/reportesPrinc.html')

def rep_suscripXArt(request):
	usrs = Usuario.objects.all()
	arts = Artista.objects.all()
	usuariosxartistas = Suscriptor.objects.filter(artista_id_artista = 0)
	if request.method == 'POST':
		idArtista = request.POST.get('dropdown2')
		usuariosxartistas = Suscriptor.objects.filter(artista_id_artista = idArtista)

	return render(request,'usuario/administrador/suscripXArt.html',{'usuarios':usrs,'artistas':arts,'usuariosxartistas':usuariosxartistas})


def rep_showMembresias(request):
	estado = ["activada","desactivada"]
	membresias = Membresia.objects.filter(activa = 1)
	if request.method == 'POST':
		estado = request.POST.get('dropdown1')
		print(estado)
		if estado == "activada":
			membresias = Membresia.objects.filter(activa = 1)
		else:
			membresias = Membresia.objects.filter(activa = 0)
		estado = ["activada","desactivada"]
	return render(request,'usuario/administrador/showMembresias.html',{'estados':estado,'membresias':membresias})

def rep_showMoreMemb(request):
	sql = """select usuario.nombre, count(membresia.id_membresia) as total
			 FROM usuario,membresia
			 WHERE usuario.id_usuario = membresia.usuario_id_usuario
			 group by usuario.nombre
			 order by total
			 desc;"""
	cursor = connection.cursor()
	cursor.execute(sql)
	lst_memb = cursor.fetchall()
	cursor.close()
	return render(request,'usuario/administrador/showMoreMemb.html',{'lst_memb':lst_memb})

def rep_repUsrMasDinero(request):
	sql = """select usuario.nombre,sum(membresia.precio) as total
			 from usuario,membresia
			 where usuario.id_usuario = membresia.usuario_id_usuario
			 group by usuario.nombre;"""
	cursor = connection.cursor()
	cursor.execute(sql)
	resp = cursor.fetchall()
	cursor.close()
	return  render(request,'usuario/administrador/repUsrMasDinero.html',{'resp':resp})


def rep_showXPais(request):
	paises = Pais.objects.all()
	resultUsr = None
	if request.method == 'POST':
		pais = request.POST.get('dropdown1')
		resultUsr = Usuario.objects.filter(pais_id_pais = pais)
	return render(request,'usuario/administrador/rep_showXPais.html',{'paises':paises,'usuarios':resultUsr})


	
def rep_showXGenero(request):
	generos = ["femenino","masculino"]
	resultUsr = None
	if request.method == 'POST':
		genero = request.POST.get('dropdown1')
		resultUsr = Usuario.objects.filter(genero = genero)
	return render(request,'usuario/administrador/showXGenero.html',{'generos':generos,'usuarios':resultUsr})	


def rep_showRepLog(request):
	sql = """SELECT * FROM USR_LOG;"""
	cursor = connection.cursor()
	cursor.execute(sql)
	lst_lst_logs = cursor.fetchall()
	cursor.close()
	for losgs in lst_lst_logs:
		print (losgs)
	return render(request,'usuario/administrador/showRepLog.html',{'lst_lst_logs':lst_lst_logs})

def rep_showCanPopu(request):
	sql = """select cancion.nombre,count(play.id_play) as play_song
			 from cancion,play
			 where cancion.id_cancion = play.cancion_id_cancion
			 group by cancion.nombre
			 order by play_song
			 desc;"""
	cursor = connection.cursor()
	cursor.execute(sql)
	logs_lst = cursor.fetchall()
	cursor.close()
	return render(request,'usuario/administrador/showCanPopu.html',{'logs_lst':logs_lst})

#================================================ USUARIO REPORTES ===============================
def showRepPrincipal(request):
	return render(request,'usuario/premium/showRepPrincipal.html')

def rep_showArtXPaisPrem(request):
	paises = Pais.objects.all()
	resultArt = None
	if request.method == 'POST':
		pais = request.POST.get('dropdown1')
		resultArt = Artista.objects.filter(pais_id_pais = pais)
	return render(request,'usuario/premium/showArtXPais.html',{'paises':paises,'artistas':resultArt})


def rep_showArtXEdad(request):
	arts = None
	cond = ["mayor","menor"]
	if request.method == 'POST':
		edadArt = forms.form_anioArtista(request.POST)
		edadDef = 24;
		if edadArt.is_valid():
			edad = edadArt.cleaned_data['anios']
		resulCond =  request.POST.get('dropdown1')
		edadStr = str(edad)
		signCond = ">"
		if resulCond == "mayor":
			signCond = ">"
		else:
			signCond = "<"
		print(signCond)
		print(resulCond)
		sql = """select artista.nombre,(MONTHS_BETWEEN(SYSDATE,artista.fecha_nacimiento)/12) as edad
				 FROM DUAL,artista
				 where (MONTHS_BETWEEN(SYSDATE,artista.fecha_nacimiento)/12)"""+ signCond + edadStr +"""
				 order by edad,nombre;"""
		print(sql)
		cursor = connection.cursor()
		cursor.execute(sql)
		arts = cursor.fetchall()
		cursor.close()
	else:
		edadArt = forms.form_anioArtista()
	return render(request,'usuario/premium/showArtXEdad.html',{'edad':edadArt,'cond':cond,'artistas':arts})


def rep_showArtXGeneroPrem(request):
	generos = Genero.objects.all()
	canciones = None
	resultArt = None
	if request.method == 'POST':
		genero = request.POST.get('dropdown1')
		canciones = Cancion.objects.filter(genero_id_genero = genero)
	return render(request,'usuario/premium/showArtXGenero.html',{'generos':generos,'canciones':canciones})	


def showCanXGeneroPrem(request):
	lstGen = Genero.objects.all()
	lstOrden = ["a-z","z-a"]
	lstCan = None
	if request.method == 'POST':
		idGen = request.POST.get('dropdown1')
		orden = request.POST.get('dropdown2')
		tipoOrden = "a-z"
		if orden == "a-z": #asc
			lstCan = Cancion.objects.filter(genero_id_genero = idGen).order_by('nombre')
		else: #desc
			lstCan = Cancion.objects.filter(genero_id_genero = idGen).order_by('-nombre')
	return render(request,'usuario/premium/showCanXGenero.html',{'canciones':lstCan,'lstGen':lstGen,'lstOrden':lstOrden})

	

def showAlbumXGenero(request):
	lstGen = Genero.objects.all()
	lstOrden = ["a-z","z-a"]
	lstAlbm = None
	if request.method == 'POST':
		idGen = request.POST.get('dropdown1')
		orden = request.POST.get('dropdown2')
		tipoOrden = "a-z"
		if orden == "a-z": #asc
			lstAlbm = Album.objects.filter(genero_id_genero = idGen).order_by('nombre')
		else: #desc
			lstAlbm = Album.objects.filter(genero_id_genero = idGen).order_by('-nombre')
	return render(request,'usuario/premium/showAlbumXGenero.html',{'lstAlbm':lstAlbm,'lstGen':lstGen,'lstOrden':lstOrden})
