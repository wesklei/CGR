#include <opencv2/opencv.hpp>
#include <stdio.h>

#define IN_IMG_ORIGEM "freedom_original.jpg"
#define IN_IMG_TEMPLATE "freedom_template.jpg"
#define OUT_IMG_SAIDA "freedom_match.jpg"

int main(int argc, char** argv){
    IplImage *img_src, *templ, *ftmp; 
    int i;
    char img_origem[] = IN_IMG_ORIGEM;
    char img_template[] = IN_IMG_TEMPLATE;
    int width, height;
    CvPoint min_loc, max_loc;
    double min_val, max_val;
    static CvScalar colors[] = {
           {{0,0,255}},{{0,255,0}},{{255,0,0}}};
    CvPoint p;
    
   //ETAPA1: carregando imagems e preparando saida
    img_src = cvLoadImage( img_origem, -1 );
    templ = cvLoadImage( img_template, -1 );
    width = img_src->width-templ->width + 1;
    height = img_src->height-templ->height +1;
    ftmp=cvCreateImage(cvSize(width, height), 32, 1);
    
    //ETAPA2: Realizando Matching 
    //usando CV_TM_CCOEFF
    cvMatchTemplate(img_src, templ, ftmp, CV_TM_CCOEFF);
    cvMinMaxLoc(ftmp, &min_val, &max_val, &min_loc, &max_loc, NULL);
    
    //marcando imagem na posicao do matching, faz retangulo vermelho
    p.x=max_loc.x+templ->width;
    p.y=max_loc.y+templ->height;
    cvRectangle(img_src, max_loc, p, colors[0], 4, 8, 0);
    
    //ETAPA3: Mostrando saida em janela
    cvNamedWindow( "Template Freedom", 1 );
    cvShowImage( "Template Freedom", templ );
    cvNamedWindow( "Imagem Original com Matching", 1 );
    cvShowImage( "Imagem Original com Matching", img_src );
    
    //Salva resultado, imagem marcada em vermelho
    cvSaveImage(OUT_IMG_SAIDA,img_src);
    
    //posicoes encontradas
    printf("CV_TM_CCOEFF: %d\n posicao superior [%d, %d]\n posicao inferior [%d, %d] \n",CV_TM_CCOEFF,max_loc.x,max_loc.y,min_loc.x,min_loc.y);
    //nao fecha, aguarda para ficar mostrando a imagem
     cvWaitKey(0);
    return(0);
}
