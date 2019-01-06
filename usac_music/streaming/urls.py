from django.conf.urls import  url

from streaming import views

urlpatterns = [
    url(r'^login/$',views.login,name='login'),
    url(r'^logout$',views.logout,name='logout'),
    url(r'^index/$',views.index,name='index'),
    url(r'^registro_usr/$',views.registro_usr,name='registro_usr'),
    url(r'^registro/$',views.registro_normal,name='registro_normal'),
    url(r'^administrador/$',views.administrador,name='administrador'),
    url(r'^archivoCSV/$',views.archivoCSV,name='archivoCSV'),
    url(r'^crud_usuario/$',views.crud_usuario,name='crud_usuario'),
    url(r'^mostrar_usuarios/$',views.crud_show_usr,name='crud_show_usr'),
    url(r'^modificar_usuarios/$',views.crud_modif_usr,name='crud_modif_usr'),
    url(r'^$', views.home, name='home'),

]