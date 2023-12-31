/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/
#include <qapplication.h>
#include "qsound.h"
#include "qsound_p.h"
#include <private/qt_mac_p.h>
#include <qhash.h>
#include <qdebug.h>
#import <AppKit/AppKit.h>

#include <AppKit/NSSound.h>

QT_BEGIN_NAMESPACE

void qt_mac_beep() 
{
    NSBeep();
}

QT_END_NAMESPACE

#ifndef QT_NO_SOUND

QT_BEGIN_NAMESPACE

typedef QHash<QSound *, NSSound const *> Sounds;
static Sounds sounds;

class QAuServerMac : public QAuServer
{
    Q_OBJECT
public:
    QAuServerMac(QObject* parent) : QAuServer(parent) { }
    void play(const QString& filename);
    void play(QSound *s);
    void stop(QSound*);
    bool okay() { return true; }
    using QAuServer::decLoop; // promote to public.
protected:
    NSSound *createNSSound(const QString &filename, QSound *qSound);
};

QT_END_NAMESPACE

QT_USE_NAMESPACE

@interface QMacSoundDelegate : NSObject {
    QSound *qSound; // may be null.
    QAuServerMac* server;
} 
-(id)initWithQSound:(QSound*)sound:(QAuServerMac*)server;
-(void)sound:(NSSound *)sound didFinishPlaying:(BOOL)aBool;
@end

@implementation QMacSoundDelegate
-(id)initWithQSound:(QSound*)s:(QAuServerMac*)serv {
    self = [super init];
    if(self) {
        qSound = s;
        server = serv;
    }
    return self;
}

// Delegate function that gets called each time a sound finishes.
-(void)sound:(NSSound *)sound didFinishPlaying:(BOOL)finishedOk
{
    // qSound is null if this sound was started by play(QString),
    // in which case there is no corresponding QSound object.
    if (qSound == 0) {
        [sound release];
        [self release];
        return;
    }

    // finishedOk is false if the sound cold not be played or
    // if it was interrupted by stop().
    if (finishedOk == false) {
        sounds.remove(qSound);
        [sound release];
        [self release];
        return;
    }

    // Check if the sound should loop "forever" (until stop).
    if (qSound->loops() == -1) { 
        [sound play];
        return;
    }

    const int remainingIterations = server->decLoop(qSound);
    if (remainingIterations > 0) {
        [sound play];
    } else {
        sounds.remove(qSound);
        [sound release];
        [self release];
    }
}
@end

QT_BEGIN_NAMESPACE

void QAuServerMac::play(const QString &fileName)
{
    QMacCocoaAutoReleasePool pool;
    NSSound * const nsSound = createNSSound(fileName, 0);
    [nsSound play];
}

void QAuServerMac::play(QSound *qSound)
{
    QMacCocoaAutoReleasePool pool;
    NSSound * const nsSound = createNSSound(qSound->fileName(), qSound);
    [nsSound play];
    // Keep track of the nsSound object so we can find it again in stop().
    sounds[qSound] = nsSound;
}

void QAuServerMac::stop(QSound *qSound)
{
    Sounds::const_iterator it = sounds.constFind(qSound);
    if (it != sounds.constEnd())
        [*it stop];
}

// Creates an NSSound object and installs a "sound finished" callack delegate on it.
NSSound *QAuServerMac::createNSSound(const QString &fileName, QSound *qSound)
{
    NSString *nsFileName = const_cast<NSString *>(reinterpret_cast<const NSString *>(QCFString::toCFStringRef(fileName)));
    NSSound * const nsSound = [[NSSound alloc] initWithContentsOfFile: nsFileName byReference:YES];
    QMacSoundDelegate * const delegate = [[QMacSoundDelegate alloc] initWithQSound:qSound:this];
    [nsSound setDelegate:delegate];
    return nsSound;
}

QAuServer* qt_new_audio_server()
{
    return new QAuServerMac(qApp);
}

QT_END_NAMESPACE

#include "qsound_mac.moc"

#endif // QT_NO_SOUND
