/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2020 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "Fourier.H"
#include "fvmLaplacian.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace laminarThermophysicalTransportModels
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class BasicThermophysicalTransportModel>
Fourier<BasicThermophysicalTransportModel>::Fourier
(
    const momentumTransportModel& momentumTransport,
    const thermoModel& thermo
)
:
    laminarThermophysicalTransportModel<BasicThermophysicalTransportModel>
    (
        typeName,
        momentumTransport,
        thermo
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class BasicThermophysicalTransportModel>
const dictionary&
Fourier<BasicThermophysicalTransportModel>::coeffDict() const
{
    return dictionary::null;
}


template<class BasicThermophysicalTransportModel>
bool Fourier<BasicThermophysicalTransportModel>::read()
{
    return true;
}


template<class BasicThermophysicalTransportModel>
tmp<volVectorField>Fourier<BasicThermophysicalTransportModel>::q() const
{
    return volVectorField::New
    (
        IOobject::groupName
        (
            "q",
            this->momentumTransport().alphaRhoPhi().group()
        ),
       -this->thermo().alpha()*this->alpha()*fvc::grad(this->thermo().he())
    );
}


template<class BasicThermophysicalTransportModel>
tmp<fvScalarMatrix>
Fourier<BasicThermophysicalTransportModel>::divq(volScalarField& he) const
{
    return -fvm::laplacian(this->alpha()*this->thermo().alpha(), he);
}


template<class BasicThermophysicalTransportModel>
tmp<volVectorField>Fourier<BasicThermophysicalTransportModel>::j
(
    const volScalarField& Yi
) const
{
    return volVectorField::New
    (
        IOobject::groupName
        (
            "j(" + Yi.name() + ')',
            this->momentumTransport().alphaRhoPhi().group()
        ),
       -this->thermo().alpha()*this->alpha()*fvc::grad(Yi)
    );
}


template<class BasicThermophysicalTransportModel>
tmp<fvScalarMatrix>
Fourier<BasicThermophysicalTransportModel>::divj(volScalarField& Yi) const
{
    return -fvm::laplacian(this->alpha()*this->thermo().alpha(), Yi);
}


template<class BasicThermophysicalTransportModel>
void Fourier<BasicThermophysicalTransportModel>::correct()
{
    laminarThermophysicalTransportModel
    <
        BasicThermophysicalTransportModel
    >::correct();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace laminarThermophysicalTransportModels
} // End namespace Foam

// ************************************************************************* //
