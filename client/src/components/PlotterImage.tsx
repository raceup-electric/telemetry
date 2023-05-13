import Car from './svg/car.svg';

function PlotterImage({ plottedData }: any){
    if(plottedData.startsWith("temperature.motor.")) return <Car />;

    return (
        <></>
    )
}

export default PlotterImage;