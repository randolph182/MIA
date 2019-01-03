from django.conf.urls import  url

from streaming import views

urlpatterns = [
    url(r'^login',views.login,name='login'),
    url(r'^index',views.index,name='index'),
    url(r'^registro_usr',views.registro_usr,name='registro_usr'),
    url(r'^', views.home, name='home'),

]