from django.conf.urls import  url

from streaming import views

from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    url(r'^login/$',views.login,name='login'),
    url(r'^logout$',views.logout,name='logout'),
    url(r'^index/$',views.index,name='index'),
    url(r'^registro_usr/$',views.registro_usr,name='registro_usr'),
    url(r'^registro/$',views.registro_normal,name='registro_normal'),
    url(r'^administrador/$',views.administrador,name='administrador'),
    url(r'^archivoCSV/$',views.archivoCSV,name='archivoCSV'),
    url(r'^crud_usuario/$',views.crud_usuario,name='crud_usuario'),
     url(r'^crud_artista/$',views.crud_artista,name='crud_artista'),
    url(r'^mostrar_usuarios/$',views.crud_show_usr,name='crud_show_usr'),
    url(r'^modificar_usuarios/$',views.crud_modif_usr,name='crud_modif_usr'),
    url(r'^eliminar_usuarios/$',views.admin_del_usr,name='admin_del_usr'),
    url(r'^correo_verificacion/([A-Za-z]{10})/$',views.confirm_correo,name='confirm_correo'),
    url(r'^mostrar_artistas/$',views.crud_show_artistas,name='crud_show_artistas'),
    url(r'^registro_artistas/$',views.crud_reg_art,name='crud_reg_art'),
    url(r'^modificar_artistas/$',views.crud_mod_art,name='crud_mod_art'),
    url(r'^eliminar_artistas/$',views.admin_del_art,name='admin_del_art'),
    url(r'^crud_canciones/$',views.crud_canciones,name='crud_canciones'),
    url(r'^mostrar_canciones/$',views.crud_show_canciones,name='crud_show_canciones'),
    url(r'^modificar_canciones/$',views.crud_mod_canciones,name='crud_mod_canciones'),
    url(r'^usuario_normal/$',views.usuarioNormal,name='usuarioNormal'),
    url(r'^usr_normal_canciones/$',views.un_show_songs,name='un_show_songs'),
    url(r'^$', views.home, name='home'),

]+ static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)