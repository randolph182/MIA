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
    url(r'^correo_verificacion/(?P<value>[a-zA-Z]+)/$',views.confirm_correo,name='confirm_correo'),
    url(r'^mostrar_artistas/$',views.crud_show_artistas,name='crud_show_artistas'),
    url(r'^registro_artistas/$',views.crud_reg_art,name='crud_reg_art'),
    url(r'^modificar_artistas/$',views.crud_mod_art,name='crud_mod_art'),
    url(r'^eliminar_artistas/$',views.admin_del_art,name='admin_del_art'),
    url(r'^crud_canciones/$',views.crud_canciones,name='crud_canciones'),
    url(r'^mostrar_canciones/$',views.crud_show_canciones,name='crud_show_canciones'),
    url(r'^modificar_canciones/$',views.crud_mod_canciones,name='crud_mod_canciones'),
    url(r'^usuario_normal/$',views.usuarioNormal,name='usuarioNormal'),
    url(r'^usr_normal_canciones/$',views.un_show_songs,name='un_show_songs'),
    url(r'^play_songs/(?P<value>\d+)/$',views.play_songs,name='play_songs'),
    url(r'^buy_membresia/$',views.buy_membresia,name='buy_membresia'),
    url(r'^usuario_premium/$',views.usuarioPremium,name='usuarioPremium'),
    url(r'^show_canciones_prem/$',views.un_show_songsPrem,name='un_show_songsPrem'),
    url(r'^show_artistas/(?P<value>\d+)/$',views.show_artistas,name='show_artistas'),
    url(r'^show_usr_membresia/(?P<value>\d+)/$',views.show_usr_membresia,name='show_usr_membresia'),
    url(r'^show_repoAdmin/$',views.show_repoAdmin,name='show_repoAdmin'),
    url(r'^rep_suscripXArt/$',views.rep_suscripXArt,name='rep_suscripXArt'),
    url(r'^rep_showMembresias/$',views.rep_showMembresias,name='rep_showMembresias'),
    url(r'^rep_repUsrMasDinero/$',views.rep_repUsrMasDinero,name='rep_repUsrMasDinero'),
    url(r'^rep_showXPais/$',views.rep_showXPais,name='rep_showXPais'),
    url(r'^rep_showXGenero/$',views.rep_showXGenero,name='rep_showXGenero'),
    url(r'^rep_showRepLog/$',views.rep_showRepLog,name='rep_showRepLog'),
    url(r'^rep_showCanPopu/$',views.rep_showCanPopu,name='rep_showCanPopu'),
    url(r'^rep_showMoreMemb/$',views.rep_showMoreMemb,name='rep_showMoreMemb'),
    url(r'^showRepPrincipal/$',views.showRepPrincipal,name='showRepPrincipal'),
    url(r'^rep_showArtXPaisPrem/$',views.rep_showArtXPaisPrem,name='rep_showArtXPaisPrem'),
    url(r'^rep_showArtXEdad/$',views.rep_showArtXEdad,name='rep_showArtXEdad'),
    url(r'^rep_showArtXGeneroPrem/$',views.rep_showArtXGeneroPrem,name='rep_showArtXGeneroPrem'),
    url(r'^showCanXGeneroPrem/$',views.showCanXGeneroPrem,name='showCanXGeneroPrem'),
    url(r'^showAlbumXGenero/$',views.showAlbumXGenero,name='showAlbumXGenero'),
    url(r'^$', views.home, name='home'),


]+ static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT) 