#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ui_mainwindow.h"

class QBox2DWorld;
class GLScene;

namespace Phonon {
    class MediaObject;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void createWorld();
    void deleteWorld();
    void createGLScene();
    void deleteGLScene();
    void createQScene();
    void startGame();
    void restartGame();
    void setSoundSource();
    void replayMusic();
    void playSound();

private:
    Ui::MainWindow *ui;
    GLScene        *glscene;
    QTimer         *timer;
    QBox2DWorld    *world;
    //Phonon::MediaObject *_music;
    //Phonon::MediaObject *_sound;
};

#endif // MAINWINDOW_H
