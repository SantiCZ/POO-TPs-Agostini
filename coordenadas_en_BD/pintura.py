"""
ejercicio 09 - coordenadas en base de datos
clase pintura con dibujo a mano alzada, persistencia en sqlite y sistema de login.
"""

import sys
import sqlite3
import hashlib
import logging
from datetime import datetime
from collections import deque

from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout,
    QHBoxLayout, QLabel, QStatusBar, QMessageBox
)
from PyQt5.QtCore import Qt, QPoint
from PyQt5.QtGui import QPainter, QPen, QColor, QPixmap, QCursor


# ─────────────────────────────────