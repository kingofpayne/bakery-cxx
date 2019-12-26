/**
 * Copyright (C) 2013
 * Olivier Heriveaux.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Olivier Heriveaux
 */


/* This is a special define which makes boost::spirit use the version 3 of
 * boost::phoenix.
 *
 * This has been added after trying to compile with the version 1.53 of boost
 * (compilation was working in boost 1.44 and boost 1.49). It seems some changes
 * in boost::variant were made and phoenix old version was no more able to use
 * the dereference operator *, which is used is most grammar files !. The define
 * below seems to fix compilation issues. */


#define BOOST_SPIRIT_USE_PHOENIX_V3 1

