XCOMM!SHELL_CMD
XCOMM
XCOMM $Xorg: Xsession,v 1.4 2000/08/17 19:54:17 cpqbld Exp $
XCOMM $XFree86: xc/programs/xdm/config/Xsession,v 1.2 1998/01/11 03:48:32 dawes Exp $

XCOMM redirect errors to a file in user's home directory if we can
for errfile in "$HOME/.xsession-errors" "${TMPDIR-/tmp}/xses-$USER" "/tmp/xses-$USER"
do
	if ( cp /dev/null "$errfile" 2> /dev/null )
	then
		chmod 600 "$errfile"
		exec > "$errfile" 2>&1
		break
	fi
done

case $# in
1)
	case $1 in
	failsafe)
		exec BINDIR/xterm -geometry 80x24-0-0
		;;
	esac
esac

XCOMM The startup script is not intended to have arguments.

startup=$HOME/.xsession
resources=$HOME/.Xresources

if [ -s "$startup" ]; then
	if [ -x "$startup" ]; then
		exec "$startup"
	else
		exec /bin/sh "$startup"
	fi
else
	if [ -r "$resources" ]; then
		BINDIR/xrdb -load "$resources"
	fi
#if defined(__SCO__) || defined(__UNIXWARE__)
        [ -r /etc/default/xdesktops ] && {
                . /etc/default/xdesktops
        }

        [ -r /etc/default/xdm ] && {
                . /etc/default/xdm
        }

        XCOMM Allow the user to over-ride the system default desktop
        [ -r $HOME/.xdmdesktop ] && {
                . $HOME/.xdmdesktop
        }

        [ -n "$XDESKTOP" ] && {
                exec `eval $XDESKTOP`
        }
#endif
	exec BINDIR/xsm
fi
