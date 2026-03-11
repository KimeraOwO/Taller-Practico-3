#include "FreakCliente.h"
#include "AIController.h"
#include "TimerManager.h"

// Constructor: Aquí configuramos las cosas antes de darle a Play
AFreakCliente::AFreakCliente()
{
	// Le decimos que sí queremos usar la función Tick
	PrimaryActorTick.bCanEverTick = true;

	bYendoAMesa = false;
	bVolviendo = false;
}

// BeginPlay: Lo primero que hace al empezar el juego
void AFreakCliente::BeginPlay()
{
	Super::BeginPlay();
	
	// Guardamos el punto exacto donde lo pusimos en el mapa (su casa)
	PuntoInicio = GetActorLocation(); 
	
	// ¡Arrancamos el ciclo!
	MoverAMesa();
}

// Tick: El cerebro que piensa todo el tiempo
void AFreakCliente::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. ¿Está yendo a la mesa y ya tenemos una mesa asignada?
	if (bYendoAMesa && PuntoMesa)
	{
		// Calculamos la distancia entre el cliente y la mesa (ignorando la altura Z para mayor precisión)
		FVector MiPosicion = GetActorLocation();
		FVector PosicionMesa = PuntoMesa->GetActorLocation();
		MiPosicion.Z = 0;
		PosicionMesa.Z = 0;

		float Distancia = FVector::Dist(MiPosicion, PosicionMesa);
		
		if (Distancia < 150.0f) // 150 cm = a un brazo de distancia
		{
			bYendoAMesa = false; // Ya no está yendo, ya llegó
			IniciarEspera();
		}
	}
	// 2. ¿Está volviendo a su punto de inicio?
	else if (bVolviendo)
	{
		FVector MiPosicion = GetActorLocation();
		FVector PosicionInicioPlana = PuntoInicio;
		MiPosicion.Z = 0;
		PosicionInicioPlana.Z = 0;

		float Distancia = FVector::Dist(MiPosicion, PosicionInicioPlana);
		
		if (Distancia < 150.0f) // Ya llegó a su casa
		{
			bVolviendo = false;
			SetActorHiddenInGame(true); // Lo hacemos invisible ("Despawn" falso para optimizar)
			
			// Esperamos 2 segundos en las sombras y luego reiniciamos todo el ciclo
			GetWorldTimerManager().SetTimer(TemporizadorEspera, this, &AFreakCliente::ReiniciarLoop, 2.0f, false);
		}
	}
}

void AFreakCliente::MoverAMesa()
{
	if (PuntoMesa)
	{
		// Conseguimos el "Cerebro" de la IA
		AAIController* ControladorIA = Cast<AAIController>(GetController());
		if (ControladorIA)
		{
			// Le damos la orden de caminar hacia la mesa
			ControladorIA->MoveToLocation(PuntoMesa->GetActorLocation());
			bYendoAMesa = true;
		}
	}
}

void AFreakCliente::IniciarEspera()
{
	// Detenemos al personaje
	AAIController* ControladorIA = Cast<AAIController>(GetController());
	if (ControladorIA)
	{
		ControladorIA->StopMovement();
	}

	// Activamos un temporizador. Cuando acabe el TiempoDeEspera, llama a VolverAInicio
	GetWorldTimerManager().SetTimer(TemporizadorEspera, this, &AFreakCliente::VolverAInicio, TiempoDeEspera, false);
}

void AFreakCliente::VolverAInicio()
{
	AAIController* ControladorIA = Cast<AAIController>(GetController());
	if (ControladorIA)
	{
		// Le ordenamos que regrese a la coordenada guardada en el BeginPlay
		ControladorIA->MoveToLocation(PuntoInicio);
		bVolviendo = true;
	}
}

void AFreakCliente::ReiniciarLoop()
{
	SetActorLocation(PuntoInicio); // Lo forzamos a su posición exacta por si se desvió un poco
	SetActorHiddenInGame(false); // Lo volvemos a hacer visible ("Respawn")
	MoverAMesa(); // Repetimos todo de nuevo
}