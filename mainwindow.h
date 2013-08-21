#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <phonon/MediaObject>
#include "worlds.h"
#include "view.h"
#include "glscene.h"

namespace Ui {
    class MainWindow;
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
    void playSound();

private:
    Ui::MainWindow *ui;
    GLScene        *glscene;
    QTimer         *timer;
    QBox2DWorld    *world;
    QBox2DView     *view;
    Phonon::MediaObject *_music;
    Phonon::MediaObject *_sound;


};

#endif // MAINWINDOW_H
