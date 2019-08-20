/*\
 * #Partie 1 : Profil d'intensite d'une ligne
 * #Brève description : Ce programme permet de tracer une ligne horizontale ou verticale sur une image et de determiner son profil d'intensité
 * #Auteur ADOUM Okim Boka, étudiant en SIM P23/IFI/UNVH
\*/
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace std;// pour utiliser la librairie standard de C++
using namespace cv;// Pour acceder aux fonctions de OpenCv

// Procédure pour tracer la ligne du profil d'intensité sur l'image originale//

void tracelign(Mat img, int choix, int valign)
/*Les variables pour représenter image et le profil d'intensité sur une matrice.
'img' pour la matrice, 'vh' pour verticale ou horizontale et 'valign' pour la valeur de la ligne*/
{
    Point M,N; //Représentation des points en 2D

    if (choix==1)//Si la ligne est horizontale
        {
        M.x = 0;
        M.y = valign;
        N.x = img.cols;
        N.y = valign;
        }
    else if(choix==2)//Si elle est verticale
        {
        M.x = valign;
        M.y = 0;
        N.x = valign;
        N.y = img.rows;
        }

    //Tracé de la ligne sur l'image
    line(img, M, N, Scalar(0, 0, 255), 1, 8);
    //Affichage du résultat
    imshow("Image originale avec ligne tracee",img);
    // Enregistrement dans un fichier image
    if(!imwrite("Tracee_ligne_image.png",img))//Nous utilisons le .png pour compresser les pixels sans modification de leur valeur
    cout<<"Erreur lors de l'enregistrement de l'image"<<endl;
}
// Fin de la procédure pour tracer la ligne du profil d'intensité sur l'image originale//


// Procédure pour tracer la courbe du profil d'intensité de la ligne //

void printim(Mat img, int choix, int valign)
/*Déclaration des variables pour determiner la matrice de l'image de la courbe du profil d,intensité et la ligne à tracer.
'img' pour la matrice, 'vh' pour verticale ou horizontale et 'valign' pour la valeur de la ligne*/
{
    Point M,N;//Représentation des points en 2D
        int lfc;//Déclaration de la variable pour determiner la largeur de la fenêtre de la courbe du profil d'intensité

    Vec3b valpxdb; //Structuration des pixels au début
    Vec3b valpxfn; //Structuration des pixels à la fin

    // Profil d'intensité lorsque la ligne est horizontale (H) ou verticale (V)
    if(choix==1)//
        {
            lfc=img.cols;// On prend toute la colonne de l'image
        }
    else if(choix==2)//
        {
            lfc=img.rows;// Ici, on prend toute la ligne de l'image
        }

    // Image de la courbe du profil d'intensité de la ligne tracée
    Mat crb_print (256,lfc,CV_8UC3,Scalar(0,0,0));//Déclaration de la variable pour determiner la matrice de la courbe du profil d'intensité
    /*Le 8U signifie que les entiers sont codés sur 8 bits,
        C3 signifie les 3 canaux pour les couleurs BVR et Scalar (0, 0, 0) est la valeur initiale pour chaque pixel */
    for(int p=0;p< lfc-1;p++)
        {    //Recupération des valeurs du pixel//
        if(choix==1)// Cas de la ligne horizontale
            {
                valpxdb=img.at<Vec3b>(valign, p);
                valpxfn=img.at<Vec3b>(valign,p+1);
            }
        else if(choix==2)// Cas de la ligne verticale
            {
                valpxdb=img.at<Vec3b>(p, valign);
                valpxfn=img.at<Vec3b>(p+1,valign);
            }
        for(int q=0; q<3;q++)
            {
                M.x = p;
                N.x = p+1;
                M.y = 255 - valpxdb.val[q];
                N.y = 255 - valpxfn.val[q];

                //Profil de la ligne correspondant à la couleur Bleue//
                if(q==0) line(crb_print,M,N,Scalar(255,0,0),1,8);

                //Profil de la ligne correspondant à la couleur Rouge//
                if(q==1) line(crb_print,M,N,Scalar(0,255,0),1,8);

                //Profil de la ligne correspondant à la couleur Verte//
                if(q==2) line(crb_print,M,N,Scalar(0,0,255),1,8);
            }
        }
        imshow("Courbe du profil d'Intensite",crb_print);//Pour l'affichage automatique de l'image de la courbe
        //Sauvergarde dans un fichier image
        if(!imwrite("Courbe_profil_intensite.png",crb_print))//Nous utilisons le .png pour compresser les pixels sans modification de leur valeur
        cout<<"Oups, il y a une erreur lors de la sauvegarde du fichier\n"<<endl;
}
// Fin de la procédure pour tracer la courbe du profil d'intensité de la ligne //
//Programme principal
int main( int argc, char** argv )
{
    int choix=0;
    char nomimg[150];
    Mat img_org; //Déclaration de la variable pour determiner la matrice de l'image originale
    int valign;
    int test=0;
    if(argc!=1)
        {
        cout << "Oups, il y a une erreur de syntaxe. Merci de saisir seulement le nom du programme : ./print"<< endl;
        }
    else
        {
        cout << "Bienvenue dans la partie pratique de notre cours de traitement d'image, sur le profil d'intensité"<<endl;
        do
            {
            cout << "Ce programme repond à deux types de profil d'intensité sur une image, correspondants aux lignes verticales ou aux lignes horizontales!"<<endl;
            cout << "1-- Tracer une ligne horizontale sur une image"<<endl;
            cout << "2-- Tracer une ligne verticale sur une image"<<endl;
            cout << "3-- Quitter"<<endl;
            cout << "Veuillez selectionner le numéro correspondant a votre besoin: $ ";
            cin  >> choix;

            if((choix!=1)&&(choix!=2)&&(choix!=3))
                {
                cout << "Veuillez choisir un numéro valide entre 1, 2 et ou 3"<<endl;
                }
            else
            {
                if(choix==3)
                    {
                    cout << "Merci et Aurevoir !!!"<< endl;
                    return 0;
                    }
                else
                    {
                    cout << "Donner le repertoire d'accé et le nom de votre image y compris son extension (.png,.tif,.jpg...)"<<endl;
                    cout << "$";
                    cin  >> nomimg;
                    img_org=imread(nomimg,1); // chargement de l'image originale
                    if(!img_org.data)
                        {
                        cout << "Desolé, image non valide. Veillez fournir une image valide \n"<<endl;
                        }
                    else
                        {
                        cout << "Donner la valeur (numéro) de la ligne choisie"<<endl;
                        cin >> valign;

                        //Execution des différents cas
                        switch (choix)
                        {
                            case 1: // Tracée d'une ligne horizontale
                                {
                                    if(img_org.rows<valign)
                                        {
                                        cout<<"Veuillez entrer une valeur de la ligne comprise entre 1 et "<<img_org.rows<<endl;
                                        test =1;
                                        }
                                   else if(test==0)
                                        {
                                        // Tracée de la courbe du profil d'intensité
                                        printim(img_org,choix,valign);

                                        // Tracée sur l'image
                                        tracelign(img_org,choix,valign);
                                        waitKey(0);
                                        cvDestroyAllWindows();
                                        return 0;
                                        }

                                    else
                                    {
                                        cout<<"Erreur de syntaxe, Veillez verifier les enformations entrées\n"<<endl; // Erreur de syntaxe
                                        return 0;
                                    }
                                break;
                                }
                            case 2: // Tracée d'une ligne verticale
                                {
                                    if(img_org.cols<valign)
                                        {
                                        cout <<"Veuillez entrer une valeur de la ligne comprise entre 0 et "<<img_org.cols<<endl;
                                        test = 1;
                                        }
                                     else if(test==0)
                                        {
                                            // Tracée de la courbe du profil d'intensité
                                            printim(img_org,choix,valign);

                                            // Tracée sur l'image
                                            tracelign(img_org,choix,valign);
                                            waitKey(0);
                                            cvDestroyAllWindows();
                                            return 0;
                                        }
                                    else
                                        {
                                        cout<<"Erreur de syntaxe. Veillez verifier les informations entrées\n"<<endl; // Erreur de syntaxe
                                        return 0;
                                        }
                                break;
                                }
                        }
                        }

                    }
                }
            }while(choix!=3);
        }
return 0;
}
//Fin du programme principal
