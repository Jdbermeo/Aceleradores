#include <stdlib.h>
#include <stdio.h>	
#include <math.h>
#include <string.h>

float y1E(float V_ant);

float y2E(float E, float w,float t);

float x1B(float V_ant);

float x2B(float x, float y,float vx,float vy, float B);

float y1B(float V_ant);

float y2B(float x, float y,float vx,float vy, float B);

int main(int argc, char **argv)
{
  //Constantes con las que se trabajara el sistema como la carga, masa, magnitud del campo magnetico, electrico y el tamaño de cada paso.
  //Relacion carga/masa es 1.758820150(44)×10{11 C/kg
  //Campo magnético es de 0.5 T

  //Declarar el puente para escribir los resultados
  FILE *out1,*out2,*out3;
  
  //El campo electrico es de 10000 N/C
  double dt = M_PI*2/(atof(argv[1]));  // En ns 
  double B = 0.5;
  float E = 9400;
  double w = B; // q/c = 1
  double periodo = (M_PI)/w; // Periodo de media vuelta en ns

 //Definir radio maximo del ciclotron, separacion entre las Ds
  float r_MAX = 10000;
  float d = 5.0;
  int salir = 0;

  //Declarar variables
  int n = 0;
  int m = 500;
  int k = 0;
  int NUM_PASOS = atoi(argv[2]);
  int NUM_VUELTAS = 0;
  double t = 0;  // En ns  
  double kx11,kx21,kx12,kx22,kx13,kx23,kx14,kx24,promkx1,promkx2;
  double ky11,ky21,ky12,ky22,ky13,ky23,ky14,ky24,promky1,promky2;
  double y1,vy1,y2,vy2,y3,vy3,x1,vx1,x2,vx2,x3,vx3;

  //Definir los vectores que van a guardar la evolucion del sistema
  double  *x,*y,*vx,*vy; 
 
  //Inicializar los primeros 500 puntos de los punteros
  x = malloc(sizeof(double)*m);
  y = malloc(sizeof(double)*m);
  vx = malloc(sizeof(double)*m);
  vy = malloc(sizeof(double)*m);
  
  //Dar las condiciones inciales al sistema
  vy[0] = 10; 
  x[0] = -(0.5*vy[0])/B;
  y[0] = d/2;
  vx[0] = 0;

  //Da las condiciones de simulacion del ciclotron

  printf("El campo magnetico del ciclotron es de %f T \n", B);
  printf("El campo elecetrico es de %fN/C \n", E);
  printf("El periodo de media vuelta en el ciclotron es de %f\n", periodo);
  printf("La frecuencia de ciclotron es de %frad/ns \n", w);

  while(pow(x[n]*x[n]+y[n]*y[n],0.5)<=pow(pow(r_MAX,2)+pow(d/2,2),0.5) && x[n]<=r_MAX && salir == 0)
    {
      	if(n==NUM_PASOS)
	{
	  printf("%i \n",n);
	  salir = 1;
	} 
      //Ajustar el tamaño de los punteros en caso que las simualcion requiera mas pasos
      if(n==(m-1))
	{
	  m= m+500;
	  x = realloc(x,sizeof(double)*m);
	  y = realloc(y,sizeof(double)*m);
	  vx = realloc(vx,sizeof(double)*m);
	  vy = realloc(vy,sizeof(double)*m);
	 }   
      
      // Evolucion del sistema si se encuentra en la region 1: CAMPO MAGNETICO 
      if(y[n]>=d/2 || y[n]<=-d/2)
	{
	  //PRIMER PASO
	  kx11 = x1B(vx[n]);//Para x
	  ky11 = y1B(vy[n]);//Para y	
	  kx21 = x2B(x[n], y[n], vx[n], vy[n], B );
	  ky21 = y2B(x[n], y[n], vx[n], vy[n], B ); 
	   
	  //Cambio en posicion	
	  x1 = x[n]+(dt/2)*kx11;	
	  y1 = y[n]+(dt/2)*ky11;
	 
	  //Cambio en velocidad
	  vx1 = vx[n]+(dt/2)*kx21;
	  vy1 = vy[n]+(dt/2)*ky21;
	  
	  //SEGUNDO PASO
	  kx12 = x1B(vx1);
	  ky12 = y1B(vy1);	
	  kx22 = x2B(x1,y1,vx1,vy1,B);
	  ky22 = y2B(x1,y1,vx1,vy1,B); 
	  
	  //Cambio en posicion	
	  x2 = x[n]+(dt/2)*kx12;	
	  y2 = y[n]+(dt/2)*ky12;
	 
	  //Cambio en velocidad
	  vx2 = vx[n]+(dt/2)*kx22;
	  vy2 = vy[n]+(dt/2)*ky22;
	  
	
	  //TERCER PASO
	  kx13 = x1B(vx2);
	  ky13 = y1B(vy2);	
	  kx23 = x2B(x2,y2,vx2,vy2,B);
	  ky23 = y2B(x2,y2,vx2,vy2,B); 
 
	  //Cambio en posicion	
	  x3 = x[n]+(dt)*kx13;	
	  y3 = y[n]+(dt)*ky13;
	 
	  //Cambio en velocidad
	  vx3 = vx[n]+(dt)*kx23;
	  vy3 = vy[n]+(dt)*ky23;
	   
	  //CUARTO PASO
	  kx14 = x1B(vx3);
	  ky14 = y1B(vy3);	
	  kx24 = x2B(x3,y3,vx3,vy3,B);
	  ky24 = y2B(x3,y3,vx3,vy3,B); 
	 
	  //PROMEDIO 
	  promkx1 = (1.0/6.0)*(kx11 + 2.0*kx12 + 2.0*kx13 + kx14);
	  promkx2 = (1.0/6.0)*(kx21 + 2.0*kx22 + 2.0*kx23 + kx24);
	  promky1 = (1.0/6.0)*(ky11 + 2.0*ky12 + 2.0*ky13 + ky14);
	  promky2 = (1.0/6.0)*(ky21 + 2.0*ky22 + 2.0*ky23 + ky24);
			
	  x[n+1] = x[n] + dt * promkx1;
	  vx[n+1] = vx[n] + dt * promkx2;
	  y[n+1] = y[n] + dt * promky1;
	  vy[n+1] = vy[n] + dt * promky2;
	}

      // Evolucion del sistema si se encuentra en la region 2: CAMPO ELECTRICO. ¿No se puede evolucionar este segmento analiticamente?
       else if(y[n]<=d/2 || y[n]>=-d/2)
	{
		printf("%f\n",n*dt);
		salir = 1;
	  //PRIMER PASO
	  ky11 = y1E(vy[n]);//Para y	
	  ky21 = y2E(E,w,t+0.5*dt); 
	   
	  //Cambio en velocidad
	  vy1 = vy[n]+(dt/2)*ky21;
	 
	  //SEGUNDO PASO
	  ky12 = y1E(vy1);	
	  ky22 = y2E(E,w,t+0.5*dt); 
	
	  //TERCER PASO
	  ky13 = y1E(vy2);	
	  ky23 = y2E(E,w,t+dt); 
 
	  //Cambio en posicion	
	  y3 = y[n]+(dt)*ky13;
	 
	  //Cambio en velocidad
	  vy3 = vy[n]+(dt)*ky23;
	   
	  //CUARTO PASO
	  ky14 = y1E(vy3);	
	  ky24 = y2E(E,w,t+dt); 
	 
	  //PROMEDIO 
	  promky1 = (1.0/6.0)*(ky11 + 2.0*ky12 + 2.0*ky13 + ky14);
	  promky2 = (1.0/6.0)*(ky21 + 2.0*ky22 + 2.0*ky23 + ky24);
			
	  x[n+1] = x[n];
	  vx[n+1] = vx[n];
	  y[n+1] = y[n] + dt * promky1;
	  vy[n+1] = vy[n] + dt * promky2;
	}

      n++;
      t+=dt;
    }
	
	char str1[50] = "posicion.dat";
	char str2[50] = "radio.dat";
	char str3[50] = "Energia.dat";

	out1 = fopen(str1,"w");
	out2 = fopen(str2,"w");
	
	fprintf(out1,"%f %f %f\n",x[(0)],y[(0)],0.0);
	fprintf(out2,"%f %f\n",pow(x[(0)]*x[(0)]+y[(0)]*y[(0)],0.5),0.0);
	
	out1 = fopen(str1,"a");
	out2 = fopen(str2,"a");	
//Exportar los datos para graficar.

  for(k=0;k<n-1;k++)
    {	
	if(k%10==0)
	{
		fprintf(out1,"%f %f %f\n",x[(k)],y[(k)],k*dt);
		fprintf(out2,"%f %f\n",k*dt,pow(x[(0)]*x[(0)]+y[(0)]*y[(0)],0.5));
    	}
    }

  return 0;
}

float y1E(float V_ant)
{
	return V_ant;
}

float y2E(float E, float w, float t)
{
  return E*cos(w*t);
}

float x1B(float V_ant)
{
	return V_ant;
}

float x2B(float x, float y, float vx, float vy, float B)
{
  return -pow(vx*vx+vy*vy,.5)*B*x/pow(x*x+y*y,0.5);
}

float y1B(float V_ant)
{
	return V_ant;
}

float y2B(float x, float y,float vx, float vy, float B)
{
  return -pow(vx*vx+vy*vy,.5)*B*y/pow(x*x+y*y,0.5);
}


