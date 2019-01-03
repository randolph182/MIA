# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='AlbmSong',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
            ],
            options={
                'db_table': 'albm_song',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Album',
            fields=[
                ('id_album', models.BigIntegerField(serialize=False, primary_key=True)),
                ('nombre', models.CharField(max_length=225, null=True, blank=True)),
                ('fecha_lanzamiento', models.DateField(null=True, blank=True)),
                ('fotografia', models.CharField(max_length=600, null=True, blank=True)),
            ],
            options={
                'db_table': 'album',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Artista',
            fields=[
                ('id_artista', models.BigIntegerField(serialize=False, primary_key=True)),
                ('nombre', models.CharField(max_length=300, null=True, blank=True)),
                ('fecha_nacimiento', models.DateField(null=True, blank=True)),
                ('fotografia', models.CharField(max_length=600, null=True, blank=True)),
            ],
            options={
                'db_table': 'artista',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Cancion',
            fields=[
                ('id_cancion', models.BigIntegerField(serialize=False, primary_key=True)),
                ('nombre', models.CharField(max_length=300, null=True, blank=True)),
                ('fecha_lanzamiento', models.DateField(null=True, blank=True)),
            ],
            options={
                'db_table': 'cancion',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='DjangoMigrations',
            fields=[
                ('id', models.IntegerField(serialize=False, primary_key=True)),
                ('app', models.TextField(null=True, blank=True)),
                ('name', models.TextField(null=True, blank=True)),
                ('applied', models.DateTimeField()),
            ],
            options={
                'db_table': 'django_migrations',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Genero',
            fields=[
                ('id_genero', models.BigIntegerField(serialize=False, primary_key=True)),
                ('nombre', models.CharField(max_length=150, null=True, blank=True)),
            ],
            options={
                'db_table': 'genero',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Membresia',
            fields=[
                ('id_membresia', models.BigIntegerField(serialize=False, primary_key=True)),
                ('fecha_inicio', models.DateField(null=True, blank=True)),
                ('fecha_fin', models.DateField(null=True, blank=True)),
                ('precio', models.DecimalField(null=True, max_digits=3, decimal_places=2, blank=True)),
                ('activa', models.BigIntegerField(null=True, blank=True)),
                ('tarjeta', models.BigIntegerField(null=True, blank=True)),
            ],
            options={
                'db_table': 'membresia',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Pais',
            fields=[
                ('id_pais', models.BigIntegerField(serialize=False, primary_key=True)),
                ('nombre', models.CharField(max_length=225, null=True, blank=True)),
            ],
            options={
                'db_table': 'pais',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Play',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
            ],
            options={
                'db_table': 'play',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='PlayList',
            fields=[
                ('id_play_list', models.BigIntegerField(serialize=False, primary_key=True)),
                ('nombre', models.CharField(max_length=150, null=True, blank=True)),
                ('fecha_creacion', models.DateField(null=True, blank=True)),
            ],
            options={
                'db_table': 'play_list',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='PlstSong',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
            ],
            options={
                'db_table': 'plst_song',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Seguidor',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
            ],
            options={
                'db_table': 'seguidor',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Suscriptor',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
            ],
            options={
                'db_table': 'suscriptor',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='UsrLog',
            fields=[
                ('id_usr_log', models.BigIntegerField(serialize=False, primary_key=True)),
                ('fecha', models.DateField(null=True, blank=True)),
                ('descripcion', models.CharField(max_length=600, null=True, blank=True)),
                ('tipo_accion', models.CharField(max_length=60, null=True, blank=True)),
            ],
            options={
                'db_table': 'usr_log',
                'managed': False,
            },
        ),
        migrations.CreateModel(
            name='Usuario',
            fields=[
                ('id_usuario', models.BigIntegerField(serialize=False, primary_key=True)),
                ('nombre', models.CharField(max_length=75, null=True, blank=True)),
                ('apellidos', models.CharField(max_length=90, null=True, blank=True)),
                ('password', models.CharField(max_length=90, null=True, blank=True)),
                ('correo', models.CharField(max_length=150, null=True, blank=True)),
                ('telefono', models.BigIntegerField(null=True, blank=True)),
                ('fotografia', models.CharField(max_length=600, null=True, blank=True)),
                ('genero', models.CharField(max_length=30, null=True, blank=True)),
                ('fecha_nacimiento', models.DateField(null=True, blank=True)),
                ('fecha_registro', models.DateField(null=True, blank=True)),
                ('direccion', models.CharField(max_length=195, null=True, blank=True)),
                ('rol', models.CharField(max_length=60, null=True, blank=True)),
                ('activo', models.BigIntegerField(null=True, blank=True)),
            ],
            options={
                'db_table': 'usuario',
                'managed': False,
            },
        ),
    ]
