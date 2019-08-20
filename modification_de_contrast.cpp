////////////////////////////////////////////////////////////////////////////////////////////////////
////             TP1 2eme Partie: Programme de modification du contraste	                    ////
////																							////
////             Auteur: ADOUM Okim Boka                            	                     	////
////             Promotion: Promotion 23 - IFI                                 					////
////																							////
////             Compilation: 1- make      												 		////
////						  2- ./main                                                      	////
////																							////
////			 Description:  Ce programme permet de modifier le contraste d'une image.	    ////
////			               	 																////
////						   																	////
////																							////
////////////////////////////////////////////////////////////////////////////////////////////////////



#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv; // Pour utiliser le librairie OpenCv
using namespace std; // Permet d'utiliser le librairie standard de C++

// Fonction de l'histogramme d'une image
Mat histoimg(Mat img)
{
    //Séparation de l'image source dans ses trois plans Bleu, Vert et Rouge
    vector<Mat> plnBVR; // Récuperation des cannaux des couleurs Bleu, vert et rouge
    split(img, plnBVR); //Division de l'image dans chaque canal de couleur

    //Etablissement du nombre de bacs
    int tailhisto=256;// de 0 à 255

    //Définition de la plage de valeurs (0 à 255)
    float intrvl[]={0,256} ;//La limite supérieure (256) est exclusive
    const float* intrvlhisto={intrvl};

    //Création des matrices pour sauvegarder les histogrammes pour chaque niveau de couleur
    Mat histoB,histoV,histoR;

    //Uniformisation de la taille des bacs et effaçage de l'histogramme au début
    bool uni=true;
    bool acc=false;

    //Calcul de l'histogramme pour chaque niveau de couleur avec la fonction calcHist
    calcHist(&plnBVR[0],1,0,Mat(),histoB,1,&tailhisto,&intrvlhisto,uni,acc);
    calcHist(&plnBVR[1],1,0,Mat(),histoV,1,&tailhisto,&intrvlhisto,uni,acc);
    calcHist(&plnBVR[2],1,0,Mat(),histoR,1,&tailhisto,&intrvlhisto,uni,acc);
    /*
    &plnBVR[0]: Le tableau source
    1 : Le nombre tableau source (dans ce cas , nous utilisons 1)
    0 : Le canal ( dim ) à mesurer
    Mat () : Un masque à utiliser sur le tableau source
    histoB : La matrice où l'histogramme sera stocké
    1 : La dimensionnalité de l' histogramme.
    HISTSIZE: Le nombre de bacs par chaque dimension utilisée
    histRange: La plage de valeurs à mesurer pour chaque dimension
    */

    //Dessin de l'histogramme pour chaque cannal de couleur
    int lrgrhisto = 512;//Définition de la largeur de l'histogramme
    int htrhisto = 400;//Définition de la hauteur de l'histogramme
    int bin_w = cvRound((double)lrgrhisto/tailhisto);
    int p=2; // Pas pour l'incrémentation
    Mat crbhistoimg(htrhisto, lrgrhisto, CV_8UC3, Scalar( 0,0,0));
    /*Le 8U signifie que les entiers sont codés sur 8 bits,
    C3 signifie les 3 canaux pour les couleurs BVR et Scalar (0, 0, 0) est la valeur initiale pour chaque pixel */

    //Normalisation pour chaque niveau de couleur avec la fonction normalize
    normalize(histoB,histoB,0,crbhistoimg.rows, NORM_MINMAX, -1, Mat());
    normalize(histoV,histoV, 0, crbhistoimg.rows, NORM_MINMAX, -1, Mat());
    normalize(histoR,histoR, 0, crbhistoimg.rows,NORM_MINMAX,-1,Mat());

    //Tracage de l'histogramme pour chaque cannal de couleurs
    /*for(inti=0;i<tailhisto;i++)
    {
        rectangle(crbhistoimg, Point( p*(i), htrhisto) ,Point( p*(i+1), htrhisto - cvRound(histoB.at<float>(i)) ),Scalar( 255, 0, 0), 2, 8, 0 );
        rectangle(crbhistoimg, Point( p*(i), htrhisto) ,Point( p*(i+1), htrhisto - cvRound(histoV.at<float>(i)) ),Scalar( 0, 255, 0), 2, 8, 0 );
        rectangle(crbhistoimg, Point( p*(i), htrhisto) ,Point( p*(i+1), htrhisto - cvRound(histoR.at<float>(i)) ),Scalar( 0, 0, 255), 2, 8, 0 );
    }*/
    for(int i =1;i<tailhisto; i++)
  {
      line(crbhistoimg,Point(bin_w*(i-1),htrhisto-cvRound(histoB.at<float>(i-1))),
      Point(bin_w*(i),htrhisto-cvRound(histoB.at<float>(i))),Scalar(255,0,0),2,8,0);
      line( crbhistoimg, Point(bin_w*(i-1),htrhisto-cvRound(histoV.at<float>(i-1))),
            Point(bin_w*(i), htrhisto-cvRound(histoV.at<float>(i))),
                       Scalar(0,255, 0),2, 8, 0  );
      line( crbhistoimg,Point( bin_w*(i-1),htrhisto-cvRound(histoR.at<float>(i-1))),
                       Point( bin_w*(i), htrhisto-cvRound(histoR.at<float>(i))),
                       Scalar(0,0,255),2,8,0);
  }

    return crbhistoimg;
}


// Fonction de calcul du profil d'intensité
Mat printimg(Mat img, int vh, int valign)
{
    Point M, N;
    int lfc;
    Vec3b valpxdb;
    Vec3b valpxfn;
    // Profil d'intensité dans le cas d'une ligne horizontale
    if(vh==1)
    {
        lfc=img.cols;
    }
    else if(vh==2)
    {
        lfc=img.rows;
    }
        // Image du profil
    Mat crb_print(256,lfc,CV_8UC3,Scalar(0,0,0));
    /*Le 8U signifie que les entiers sont codés sur 8 bits,
    C3 signifie les 3 canaux pour les couleurs BVR et Scalar (0, 0, 0) est la valeur initiale pour chaque pixel */

        for(int i=0;i< lfc-1;i++)
        {    //Recuperation des valeurs du pixel
            if(vh==1)
            {
                valpxdb=img.at<Vec3b>(valign, i);
                valpxfn=img.at<Vec3b>(valign,i+1);
            }
            else if(vh==2)
            {
                valpxdb=img.at<Vec3b>(i, valign);
                valpxfn=img.at<Vec3b>(i+1,valign);
            }

            for(int j=0;j<3;j++)
            {
                M.x=i;
                N.x=i+1;
                M.y=255-valpxdb.val[j];
                N.y= 255-valpxfn.val[j];

                // Profil de la ligne correspondant à la couleur Bleue
                if(j==0) line(crb_print,M,N,Scalar(255, 0, 0), 1, 8);

                // Profil de la ligne correspondant à la couleur Rouge
                if(j==1) line(crb_print,M,N, Scalar(0, 255, 0), 1, 8);

                // Profil de la ligne correspondant à la couleur Verte
                if(j==2) line(crb_print,M,N, Scalar(0, 0, 255), 1, 8);
            }
        }

   return crb_print;
    }


//Fonction pour le traçage du trait du profil d'intensité sur l'image
Mat tracelign(Mat img, int vh, int valign)
{
    Point debut, fin;

    if (vh==1)
    {
        debut.x = 0;
        debut.y = valign;
        fin.x = img.cols;
        fin.y = valign;
    }
    else if(vh==2)
    {
        debut.x = valign;
        debut.y = 0;
        fin.x = valign;
        fin.y = img.rows;
    }

    //Tracé de la ligne sur l'image
    line(img,debut,fin,Scalar(0, 0, 255),1,8);

    return img;
}


// Fonction linéaire à trois points de transformation du contraste
Mat fctline3pts(Mat img,Point pt1, Point pt2, Point pt3)
{
    Mat img3pts(img.rows, img.cols, CV_8UC3, Scalar( 0,0,0));
    /*Le 8U signifie que les entiers sont codés sur 8 bits,
    C3 signifie les 3 canaux pour les couleurs BVR et Scalar (0, 0, 0) est la valeur initiale pour chaque pixel */

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            for(int c = 0; c < 3; c++)
            {
                    int val = img.at<Vec3b>(i,j)[c];
                    if(val>=0 && val <= pt1.x) val = (int) pt1.y/pt1.x * val;
                    else if(pt1.x < val && val <= pt2.x) val =(int) ((pt2.y - pt1.y)/(pt2.x - pt1.x)) * (val - pt1.x) + pt1.y;
                    else if(pt2.x < val && val <= pt3.x) val =(int) ((pt3.y - pt2.y)/(pt3.x - pt2.x)) * (val - pt2.x) + pt2.y;
                    else if(pt3.x < val && val <= 255) val =(int) ((255 - pt3.y)/(255 - pt3.x)) * (val - pt3.x) + pt3.y;
                    img3pts.at<Vec3b>(i,j)[c] = saturate_cast<uchar>(val);
            }
        }
    }

    //Image contrastee par la fonction linéaire à trois points
    imshow("Image contrastee par fl3pts",img3pts);
    if(!imwrite("Image contrastee par fl3pts.png",img3pts))
    cout<<"Erreur lors de l'enregistrement"<<endl;

    waitKey(10000);//Affichage de l'image pendant 3s
    destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 3s pour la suite du programme

    return img3pts;
}

// Procedure pour représenter la courbe de la fonction linéaire à trois points
void crbfctline3pts(Point pt1, Point pt2, Point pt3){

    // Declaration de l'image de la courbe
    Mat crb3pts(256,256,CV_8UC3,Scalar(0,0,0));
    /*Le 8U signifie que les entiers sont codés sur 8 bits,
    C3 signifie les 3 canaux pour les couleurs BVR et Scalar (0, 0, 0) est la valeur initiale pour chaque pixel */

    // Déclaration et initialisation des points de début et de fin
    Point debut, fin;
    debut.x = 0 ;
    debut.y = 255 ;
    fin.x = 255;
    fin.y = 0;
    pt1.y = 255 - pt1.y;
    pt2.y = 255 - pt2.y;
    pt3.y = 255 - pt3.y;

    // Tracé des lignes de la courbe
    line(crb3pts, debut, pt1, Scalar(255, 0, 0), 1, 8);
    line(crb3pts, pt1, pt2, Scalar(255, 0, 0), 1, 8);
    line(crb3pts, pt2, pt3, Scalar(255, 0, 0), 1, 8);
    line(crb3pts, pt3, fin, Scalar(255, 0, 0), 1, 8);

    // Affichage de la courbe de la fonction
    imshow("Fonction lineaire a trois points",crb3pts);

    // Enregistrement de l'histogramme orginal dans un fichier image
    if(!imwrite("Fonction_lineaire_3pts.png", crb3pts))
    cout<<"Erreur lors de l'enregistrement"<<endl;

}

// Fonction non linéaire pour modifier le contraste suivant la correction de gamma
Mat corgam(Mat img,float g)
{

    Mat imgcorgam(img.rows, img.cols, CV_8UC3, Scalar( 0,0,0));

    for(int i=0;i<img.rows;i++)
    {

        for(int j=0;j< img.cols;j++)
        {

            for(int c=0;c<3;c++)
            {

                int val = img.at<Vec3b>(i,j)[c];
                if(val < 0) val = 0;
                else if(val > 255) val =255;
                else if(0 <= val && val <= 255)
                val = pow((val/255.0),g)*255.0;
               imgcorgam.at<Vec3b>(i,j)[c] = saturate_cast<uchar>(val);
            }
        }
    }

   //Image contrastee par gamma
    imshow("Image contrastee par gamma",imgcorgam);
    if(!imwrite("Image contrastee par gamma.png",imgcorgam))
    cout<<"Erreur lors de l'enregistrement"<<endl;

    waitKey(10000);//Affichage de l'image pendant 3s
    destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 3s pour la suite du programme

   return imgcorgam;
}


// Procedure pour la représentation de la courbe de gamma
void crbcorgam(float g){

    Mat imgcrbgam(256, 256, CV_8UC3, Scalar( 0,0,0));
    /*Le 8U signifie que les entiers sont codés sur 8 bits,
    C3 signifie les 3 canaux pour les couleurs BVR et Scalar (0, 0, 0) est la valeur initiale pour chaque pixel */

    Point pt1, pt2;

    for (float i =0.0;i<256.0;i++)
    {
        pt1.x = i;
        pt2.x = i+1;
        pt1.y = 255.0 - (pow((pt1.x/255.0),g)*255.0);
        pt2.y = 255.0 - (pow((pt2.x/255.0),g)*255.0);
        if(pt1.y < 0.0)  pt1.y = 0.0;
        if(pt1.y> 255.0) pt1.y = 255.0;
        if(pt2.y < 0.0)  pt2.y = 0.0;
        if(pt2.y> 255.0) pt2.y = 255.0;
        line(imgcrbgam,pt1,pt2, Scalar(255,0,0),1,8);
    }
     // Affichage et enregistrement de la courbe de la fonction
        imshow("Fonction gamma",imgcrbgam);
        if(!imwrite("fonction_gamma.png",imgcrbgam))
        cout<<"Erreur lors de l'enregistrement"<<endl;

    waitKey(2000);//Affichage de l'image pendant 2s
    destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 2s pour la suite du programme
}


// Fonction linéaire avec saturation (min et max)

Mat fctlinsat(Mat img, Point Min, Point Max)
{

    Mat imgsat(img.rows, img.cols, CV_8UC3, Scalar( 0,0,0));
    /*Le 8U signifie que les entiers sont codés sur 8 bits,
    C3 signifie les 3 canaux pour les couleurs BVR et Scalar (0, 0, 0) est la valeur initiale pour chaque pixel */

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            for(int c = 0; c < 3; c++)
            {
                    int val = img.at<Vec3b>(i,j)[c];
                    if(val>=0 && val <= Min.x) val = 0;
                    else if(Min.x < val && val <= Max.x)
                    val =(int) (255/(Max.x - Min.x)) * (val - Min.x);
                    else if(Max.x < val && val <= 255) val = 255;
                    imgsat.at<Vec3b>(i,j)[c] = saturate_cast<uchar>(val);
            }
        }
    }

    //Image contrastee par la saturation
    imshow("Image contrastee par saturation",imgsat);
    if(!imwrite("Image contrastee par saturation.png",imgsat))
    cout<<"Erreur lors de l'enregistrement"<<endl;

    waitKey(3000);//Affichage de l'image pendant 3s
    destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 3s pour la suite du programme


    return imgsat;
}

// Procedure pour la représentation de la fonction linéaire de saturation
void crbfctlinsat(Point Min, Point Max)
{

    // Declaration de l'image de la courbe
    Mat crbfctlinsat(256,256,CV_8UC3,Scalar(0,0,0));
    /*Le 8U signifie que les entiers sont codés sur 8 bits,
    C3 signifie les 3 canaux pour les couleurs BVR et Scalar (0, 0, 0) est la valeur initiale pour chaque pixel */

    // Déclaration et initialisation des points de début et de fin
    Point debut, fin;
    debut.x = 0 ;
    debut.y = 255 ;
    fin.x = 255;
    fin.y = 0;
    Min.y = 255 - Min.y;
    Max.y = 255 - Max.y;

    // Tracé des lignes de la courbe
    line(crbfctlinsat, debut, Min, Scalar(255, 0, 0), 1, 8);
    line(crbfctlinsat, Min, Max, Scalar(255, 0, 0), 1, 8);
    line(crbfctlinsat, Max, fin, Scalar(255, 0, 0), 1, 8);

    // Affichage de la courbe de la fonction

    imshow( "Fonction lineaire avec saturation",crbfctlinsat);

    // Enregistrement de l'histogramme orginal dans un fichier image

    if(!imwrite("fonction_saturation.png", crbfctlinsat))
    cout<<"Erreur lors de l'enregistrement"<<endl;
    waitKey(2000);//Affichage de l'image pendant 2s
    destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 2s pour la suite du programme

}



// Programme principal

int main(int argc, char** argv)
{
//Declaration des variables globales
    int choix=0;
    char nomimg[100];
    Mat imgorig;
    Mat imaamlr;
    Mat courbe;
    int vh;
    int valign;

    if(argc!=1)
    {
        cout << "Desolé, il y a une erreur de syntaxe. Merci de saisir seulement le nom du programme : ./main"<< endl;
    }
    else
    {
        cout << "Bienvenue dans le programme de modification de contraste d'une image réalisé par ADOUM Okim Boka"<<endl;
        cout << "Avant d'appliquer une fonction de transformation, veuillez determiner l'histogramme de l'image originale"<<endl;
        cout << "Veillez renseigner le chemin d'accée et le nom de votre image y compris son extension (.png,.tif,.jpg...)"<<endl;
                    cout << "$ ";
                    cin  >> nomimg;
                    imgorig=imread(nomimg,1); //Chargement de l'image originale

                    if(!imgorig.data)
                    {
                        cout << "Desolé, image non valide. Merci fournir une image valide \n"<<endl;
                    }

                    else
                        {
                        cout << "Vous avez 5 secondes pour analyser l'histogramme de l'image originale :\n"<<endl;

                        Mat histoimgori=histoimg(imgorig);//Affichage de l'histogramme de l'image originale pendant 5s

                        namedWindow("Histogramme image originale",CV_WINDOW_AUTOSIZE);
                        imshow("Histogramme image originale",histoimgori);

                        // Enregistrement du profil original dans un fichier image
                        if(!imwrite("Histo_image_originale.png",histoimgori))
                        cout<<"Erreur lors de l'enregistrement"<<endl;

                        waitKey(5000);//Affichage de l'image pendant 5s
                        destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 5s pour la suite du programme*/

                    //Suite du programme, choix de la fonction de transformation

                    do{
                        cout << "Veillez choisir la fonction correspondant à votre besoins, en choisissant un numero"<<endl;
                        cout << "1) Amélioration avec la fonction linéaire à trois points"<<endl;
                        cout << "2) Amélioration avec la fonction linéaire avec saturation (min et max)"<<endl;
                        cout << "3) Amélioration avec la correction gamma: fonction non linéaire"<<endl;
                        cout << "4) Amélioration avec l'égalisation de l'histogramme "<<endl;
                        cout << "5) Ou voulez vous quitter le programme???"<<endl;
                        cout << "Veuillez faire un choix: $ ";
                        cin  >> choix;

                        if((choix!=1)&&(choix!=2)&&(choix!=3)&&(choix!=4)&&(choix!=5))
                        {
                            cout << "Veuillez choisir un numero valide"<<endl;
                        }
                        else{
                            if(choix==5)
                            {
                                cout << " Merci et aurevoir!!!"<< endl;
                                return 0;
                            }
                            else
                                {

                        cout << "Traçage du profil d'intensité? Taper '1' pour une ligne horizontale ou '2' pour verticale" << endl;
                        cin >> vh;
                        cout << "Donner la valeur de la ligne à tracer"<<endl;
                        cin >> valign;

                        switch (choix)
                        {
                            case 1: // Amélioration avec la fonction linéaire à trois points
                                {
                                Point pt1, pt2, pt3;
                                // Collecte des informations spécifiques à la fonction linéaire
                                cout << "Amélioration du contraste avec fonction linéaire à trois (03) points"<<endl;
                                cout << " Veuillez saisir les coordonnées des trois points"<<endl;

                                cout << "Abscisse du 1er point : $ ";
                                cin  >> pt1.x;
                                cout << "Ordonnée du 1er point : $ ";
                                cin  >> pt1.y;

                                cout << "Abscisse du 2e point : $ ";
                                cin  >> pt2.x;
                                cout << "Ordonnée du 2e point : $ ";
                                cin  >> pt2.y;

                                cout << "Abscisse du 3e point: $ ";
                                cin  >> pt3.x;
                                cout << "Ordonnée du 3e point : $ ";
                                cin  >> pt3.y;

                                // Traitement de l'image selon les données receuillies
                                imaamlr=fctline3pts(imgorig,pt1,pt2,pt3);
                                crbfctline3pts(pt1,pt2,pt3);

                                break;
                            }

                            case 2: // Modification linaire avec saturation
                            {
                                Point Min, Max;
                                // Collecte des informations spécifiques à la fonction linéaires

                                cout << "Modification avec la fonction linéaire de saturation"<<endl;
                                cout << " Veuillez saisir les abscisses des deux points (Min et Max)"<<endl;

                                cout << "Abscisse Min $ ";
                                cin  >> Min.x;
                                Min.y = 0;
                                cout << "Abscisse Max $ ";
                                cin  >> Max.x;
                                Max.y = 255;

                                // Traitement de l'image selon les points Min et Max
                                imaamlr=fctlinsat(imgorig,Min,Max);

                                //Courbe de la fonction gamma
                                crbfctlinsat(Min, Max);

                                break;
                            }

                            case 3: // Amélioration avec la correction gamma: fonction non linéaire
                            {
                                float g;
                                // Collecte des informations spécifiques à la fonction linéaires

                                cout << "Amelioration du contraste avec une fonction non linéaire :la correction gamma"<<endl;
                                cout << " Veuillez saisir la valeur de gamma qui est comprise entre 0 et 1"<<endl;
                                cin  >>g;

                                // Traitement de l'image selon la valeur de gamma
                                imaamlr=corgam(imgorig,g);

                                //Courbe de la fonction gamma
                                crbcorgam(g);

                                break;
                            }

                            case 4: // Amélioration avec l'égalisation de l'histogramme
                            {
                                cout << "L'égalisation de l'histogramme est la solution par defaut pour améliorer le contraste d'image à niveau de gris\n"<<endl;
                                cout << "De plus, elle ne necessite pas plusieurs parametres, juste l'image source!!!\n"<<endl;

                                const char* fenetre_sourc = "Image Source";
                                const char* fenetre_egal = "Image egalisee";

                                // Convertion en niveaux de gris de l'image
                                Mat imgng;
                                cvtColor(imgorig,imgng,COLOR_BGR2GRAY);

                                // Application de l'égalisation de l'histogramme avec la fonction 'equalizeHist'
                                Mat imgegal;

                                equalizeHist(imgng,imgegal);

                                imaamlr==imgegal;

                                // Affichage des résultats des deux resultats

                                namedWindow( fenetre_sourc, WINDOW_AUTOSIZE );
                                namedWindow( fenetre_egal, WINDOW_AUTOSIZE );

                                imshow(fenetre_sourc,imgorig);
                                imshow(fenetre_egal,imgegal);

                                if(!imwrite("Image egalisee.png",imgegal))
                                cout<<"Erreur lors de l'enregistrement"<<endl;

                                waitKey(5000);//Affichage de l'image pendant 5s
                                destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 3s pour la suite du programme

                                break;
                            }
                        }


                        /*/ Affichage et enregistrement des images issues de l'originale /*/

                        //Profil d'intensité

                        Mat printimgori=printimg(imgorig,vh,valign);

                        imshow("Profil d'intensite issu l image originale",printimgori);
                        if(!imwrite("profi_inten_imgori.png",printimgori))
                        cout<<"Erreur lors de l'enregistrement"<<endl;
                        waitKey(2000);//Affichage de l'image pendant 2s
                        destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 5s pour la suite du programme

                        //Image avec le trait

                        Mat traceimgori=tracelign(imgorig,vh,valign);

                        imshow("Image originale avec trait",traceimgori);
                        if(!imwrite("trait_imaori.png",traceimgori))
                        cout<<"Erreur lors de l'enregistrement"<<endl;
                        waitKey(2000);//Affichage de l'image pendant 2s
                        destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 2s pour la suite du programme*/

                        ///////////////////////////

                        /*/ Affichage et enregistrement des images issues de de l'image contrastée /*/

                        // Histogramme

                        Mat histoimgcont=histoimg(imaamlr);

                        namedWindow("Histogramme image contrastee",CV_WINDOW_AUTOSIZE);
                        imshow("Histogramme image contrastee",histoimgcont);
                        if(!imwrite("Histo_image_contrastee.png",histoimgcont))
                        cout<<"Erreur lors de l'enregistrement"<<endl;
                        waitKey(5000);//Affichage de l'image pendant 5s
                        destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 5s pour la suite du programme*/

                        //Profil d'intensité

                        Mat printimgcont=printimg(imaamlr,vh,valign);

                        imshow("Profil d'intensite issu l image contrastee",printimgcont);
                        if(!imwrite("profi_inten_imgcont.png",printimgcont))
                        cout<<"Erreur lors de l'enregistrement"<<endl;
                        waitKey(2000);//Affichage de l'image pendant 2s
                        destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 5s pour la suite du programme


                        //Image avec le trait

                        Mat traceimgcont=tracelign(imaamlr,vh,valign);

                        imshow("Image contrastee avec trait",traceimgcont);
                        if(!imwrite("trait_imacont.png",traceimgcont))
                        cout<<"Erreur lors de l'enregistrement"<<endl;
                        waitKey(2000);//Affichage de l'image pendant 2s
                        destroyAllWindows();//Fermeture de la fenêtre de l'image apres les 2s pour la suite du programme*/


                        // Destruction des fenêtres
                        //cvDestroyAllWindows();

                        // Quitter le programme

                        return 0;
                    }
                }
            }while(choix!=5);
        }
    }
}

