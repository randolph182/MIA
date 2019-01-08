# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Remove `managed = False` lines if you wish to allow Django to create, modify, and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
#
# Also note: You'll have to insert the output of 'django-admin sqlcustom [app_label]'
# into your database.
from __future__ import unicode_literals

from django.db import models


class AlbmSong(models.Model):
    cancion_id_cancion = models.ForeignKey('Cancion', db_column='cancion_id_cancion')
    album_id_album = models.ForeignKey('Album', db_column='album_id_album')
    id_albm_song = models.BigIntegerField(primary_key=True)

    class Meta:
        managed = False
        db_table = 'albm_song'


class Album(models.Model):
    id_album = models.BigIntegerField(primary_key=True)
    nombre = models.CharField(max_length=225, blank=True, null=True)
    fecha_lanzamiento = models.DateField(blank=True, null=True)
    fotografia = models.CharField(max_length=600, blank=True, null=True)
    genero_id_genero = models.ForeignKey('Genero', db_column='genero_id_genero')
    artista_id_artista = models.ForeignKey('Artista', db_column='artista_id_artista')

    class Meta:
        managed = False
        db_table = 'album'


class Artista(models.Model):
    id_artista = models.BigIntegerField(primary_key=True)
    nombre = models.CharField(max_length=300, blank=True, null=True)
    fecha_nacimiento = models.DateField(blank=True, null=True)
    fotografia = models.CharField(max_length=600, blank=True, null=True)
    pais_id_pais = models.ForeignKey('Pais', db_column='pais_id_pais')

    class Meta:
        managed = False
        db_table = 'artista'


class Cancion(models.Model):
    id_cancion = models.BigIntegerField(primary_key=True)
    nombre = models.CharField(max_length=300, blank=True, null=True)
    fecha_lanzamiento = models.DateField(blank=True, null=True)
    genero_id_genero = models.ForeignKey('Genero', db_column='genero_id_genero')
    artista_id_artista = models.ForeignKey(Artista, db_column='artista_id_artista')
    ruta_cancion = models.CharField(max_length=750, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'cancion'


class Genero(models.Model):
    id_genero = models.BigIntegerField(primary_key=True)
    nombre = models.CharField(max_length=150, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'genero'


class Membresia(models.Model):
    id_membresia = models.BigIntegerField(primary_key=True)
    fecha_inicio = models.DateField(blank=True, null=True)
    fecha_fin = models.DateField(blank=True, null=True)
    precio = models.DecimalField(max_digits=3, decimal_places=2, blank=True, null=True)
    activa = models.BigIntegerField(blank=True, null=True)
    tarjeta = models.BigIntegerField(blank=True, null=True)
    usuario_id_usuario = models.ForeignKey('Usuario', db_column='usuario_id_usuario')

    class Meta:
        managed = False
        db_table = 'membresia'


class Pais(models.Model):
    id_pais = models.BigIntegerField(primary_key=True)
    nombre = models.CharField(max_length=225, blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'pais'


class Play(models.Model):
    usuario_id_usuario = models.ForeignKey('Usuario', db_column='usuario_id_usuario')
    cancion_id_cancion = models.ForeignKey(Cancion, db_column='cancion_id_cancion')
    id_play = models.BigIntegerField(primary_key=True)

    class Meta:
        managed = False
        db_table = 'play'

class PlayList(models.Model):
    id_play_list = models.BigIntegerField(primary_key=True)
    nombre = models.CharField(max_length=150, blank=True, null=True)
    fecha_creacion = models.DateField(blank=True, null=True)
    usuario_id_usuario = models.ForeignKey('Usuario', db_column='usuario_id_usuario')

    class Meta:
        managed = False
        db_table = 'play_list'


class PlstSong(models.Model):
    play_list_id_play_list = models.ForeignKey(PlayList, db_column='play_list_id_play_list')
    cancion_id_cancion = models.ForeignKey(Cancion, db_column='cancion_id_cancion')
    id_plst_song = models.BigIntegerField(primary_key=True)

    class Meta:
        managed = False
        db_table = 'plst_song'


class Seguidor(models.Model):
    usuario_id_usuario = models.ForeignKey('Usuario', db_column='usuario_id_usuario')
    play_list_id_play_list = models.ForeignKey(PlayList, db_column='play_list_id_play_list')
    id_seguidor = models.BigIntegerField(primary_key=True)

    class Meta:
        managed = False
        db_table = 'seguidor'


class Suscriptor(models.Model):
    usuario_id_usuario = models.ForeignKey('Usuario', db_column='usuario_id_usuario')
    artista_id_artista = models.ForeignKey(Artista, db_column='artista_id_artista')
    id_suscriptor = models.BigIntegerField(primary_key=True)

    class Meta:
        managed = False
        db_table = 'suscriptor'


class UsrLog(models.Model):
    id_usr_log = models.BigIntegerField(primary_key=True)
    fecha = models.DateField(blank=True, null=True)
    descripcion = models.CharField(max_length=600, blank=True, null=True)
    tipo_accion = models.CharField(max_length=60, blank=True, null=True)
    usuario_id_usuario = models.ForeignKey('Usuario', related_name='usuario_id_usuario')
    usuario_id_usuario1 = models.ForeignKey('Usuario', related_name='usuario_id_usuario1')

    class Meta:
        managed = False
        db_table = 'usr_log'


class Usuario(models.Model):
    id_usuario = models.BigIntegerField(primary_key=True)
    nombre = models.CharField(max_length=75, blank=True, null=True)
    apellidos = models.CharField(max_length=90, blank=True, null=True)
    password = models.CharField(max_length=90, blank=True, null=True)
    correo = models.CharField(max_length=150, blank=True, null=True)
    telefono = models.BigIntegerField(blank=True, null=True)
    fotografia = models.CharField(max_length=600, blank=True, null=True)
    genero = models.CharField(max_length=30, blank=True, null=True)
    fecha_nacimiento = models.DateField(blank=True, null=True)
    fecha_registro = models.DateField(blank=True, null=True)
    direccion = models.CharField(max_length=195, blank=True, null=True)
    rol = models.CharField(max_length=60, blank=True, null=True)
    pais_id_pais = models.BigIntegerField()
    token_correo = models.CharField(max_length=450, blank=True, null=True)
    tactivo = models.BigIntegerField(blank=True, null=True)

    class Meta:
        managed = False
        db_table = 'usuario'